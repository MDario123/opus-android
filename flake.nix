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
        config.android_sdk.accept_license = true;
        inherit system;
      };
    in
    {
      devShells."${system}" =
        let
          buildToolsVersion = "30.0.3";
          buildToolsVersionForAapt2 = "34.0.0";
          cmakeVersion = "3.31.6";

          androidComposition =
            (pkgs.androidenv.composeAndroidPackages {
              platformVersions = [ "34" "33" "31" "30" ];
              abiVersions = [ "armeabi-v7a" "arm64-v8a" "x86" "x86_64" ];

              includeEmulator = true;
              emulatorVersion = "34.1.9";

              includeSources = false;
              includeSystemImages = false;
              systemImageTypes = [ "google_apis_playstore" ];

              buildToolsVersions = [ buildToolsVersionForAapt2 buildToolsVersion "35.0.0" ];

              includeNDK = true;
              ndkVersions = [ "25.2.9519653" ];

              includeCmake = true;
              cmakeVersions = [ cmakeVersion ];

              useGoogleAPIs = false;
              useGoogleTVAddOns = false;
              extraLicenses = [
                "android-googletv-license"
                "android-sdk-arm-dbt-license"
                "android-sdk-license"
                "android-sdk-preview-license"
                "google-gdk-license"
                "intel-android-extra-license"
                "intel-android-sysimage-license"
                "mips-android-sysimage-license"
              ];
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
              export PATH="$(echo "$ANDROID_HOME/cmake/${cmakeVersion}".*/bin):$PATH"
              export SHELL=$(which zsh)
            '';
          };

          default = android;
        };
    };
}
