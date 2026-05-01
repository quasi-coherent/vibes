let
  perSystem =
    { self', vibes, vibelib, ... }:
    let
      vibe = vibes.stdenv.mkDerivation {
        pname = "vibe";
        # version = "vibe";
        version = "0.0.0";
        src = vibelib.cleanSource ./.;

        nativeBuildInputs = with vibes; [
          gcc
          coreutils
        ];

        buildPhase = "gcc -o vibe vibe.c";
        installPhase = ''
          mkdir -p $out/bin
          cp vibe $out/bin
        '';
      };
    in
    {
      packages = {
        inherit vibe;
        default = vibe;
      };

      devShells = {
        # vibe
        default = vibes.mkShell {
          packages = with vibes; [
            gcc
            gnumake
            gdb
            clang-tools
            nixd
          ];
        };

        vibe =
          let
            vibe = self'.packages.vibe;
          in
          vibes.mkShell {
            packages = [ vibe ];
            shellHook = "${vibelib.getBin vibe}/bin/vibe";
          };
      };
    };
in
{
  inherit perSystem;
}
