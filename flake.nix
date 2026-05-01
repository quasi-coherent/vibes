{
  description = "vibe";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  inputs.flake-parts.url = "github:hercules-ci/flake-parts";
  inputs.systems.url = "github:nix-systems/default";

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import inputs.systems;
      imports = [
        ./vibe.nix
        inputs.flake-parts.flakeModules.easyOverlay
      ];

      perSystem =
        { pkgs, ... }:
        {
          _module.args = {
            vibes = pkgs;
            vibelib = pkgs.lib;
          };

          overlayAttrs = {
            vibes = pkgs;
            vibelib = pkgs.lib;
          };
        };
    };
}
