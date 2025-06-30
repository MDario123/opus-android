{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        config = {
          allowUnfree = true;
          android_sdk.accept_license = true;
        };
        inherit system;
      };
    in
    {
      devShells."${system}" =
        let
          buildToolsVersion = "30.0.3";
          buildToolsVersionForAapt2 = "34.0.0";

          androidComposition =
            (pkgs.androidenv.composeAndroidPackages {
              platformVersions = [ "33" ];
              abiVersions = [ "armeabi-v7a" "arm64-v8a" "x86" "x86_64" ];

              includeEmulator = false;
              includeSources = false;
              includeSystemImages = false;
              includeCmake = false;

              buildToolsVersions = [ buildToolsVersionForAapt2 buildToolsVersion "35.0.0" ];

              includeNDK = true;
              ndkVersions = [ "25.2.9519653" ];

            });
        in
        rec {
          android = pkgs.mkShell rec {
            buildInputs = with pkgs; [
              androidComposition.androidsdk
              android-tools
              gradle
              jdk17

              cmake
              ninja
              clang
            ];

            ANDROID_HOME = "${androidComposition.androidsdk}/libexec/android-sdk";
            ANDROID_NDK_ROOT = "${ANDROID_HOME}/ndk-bundle";

            # Use the same buildToolsVersion here
            GRADLE_OPTS = "-Dorg.gradle.project.android.aapt2FromMavenOverride=${ANDROID_HOME}/build-tools/${buildToolsVersionForAapt2}/aapt2";

            shellHook = ''
              export SHELL=$(which zsh)
              unset ANDROID_SDK_ROOT
            '';
          };

          default = android;
        };
    };
}
