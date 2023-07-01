{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs }: let
    pkgs = nixpkgs.legacyPackages.x86_64-linux.pkgs;
  in {
    devShells.x86_64-linux.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        clang-tools
        clang
        bear

        # I copied the files from nixpkgs
        # and updated it myself.
        (pkgs.callPackage ./raylib.nix {})

        gdb

        cmake
        xorg.libX11
        xorg.libXrandr
        xorg.libXinerama
        xorg.libXcursor
        xorg.libXi
      ];
    };
  };
}
