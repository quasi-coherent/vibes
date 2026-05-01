{
  description = "vibe";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  inputs.flake-parts.url = "github:hercules-ci/flake-parts";
  inputs.systems.url = "github:nix-systems/default";

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import inputs.systems;

      perSystem =
        { self', pkgs, ... }:
        let
          vibe = pkgs.stdenv.mkDerivation {
            pname = "vibe";
            # version = "vibe";
            version = "0.0.0";
            src = pkgs.lib.cleanSource ./.;

            nativeBuildInputs = with pkgs; [
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
            default = pkgs.mkShell {
              packages = with pkgs; [
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
              pkgs.mkShell {
                packages = [ vibe ];
                shellHook = "${pkgs.lib.getBin vibe}/bin/vibe";
              };
          };
        };
    };
}
