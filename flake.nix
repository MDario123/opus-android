{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        config.allowUnfree = true;
        inherit system;
      };
    in
    {
      devShells."${system}" = rec {
        android = pkgs.mkShell {
          buildInputs = with pkgs; [
            android-studio
            android-tools
            cmake
            gradle
          ];

          shellHook = ''
            export SHELL=$(which zsh)
          '';
        };

        default = android;
      };
    };
}
