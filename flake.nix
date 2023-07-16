{
    inputs = {
        nixpkgs.url = "github:nixOS/nixpkgs/nixos-unstable";
    };

    outputs = { nixpkgs, ... }:
    let
      supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];
      forAllSystems    = nixpkgs.lib.genAttrs supportedSystems;
      nixpkgsFor       = forAllSystems (system: import nixpkgs { inherit system; });
    in {
        devShells = forAllSystems (system:
        let pkgs = nixpkgsFor.${system};
        in
        {
            default = pkgs.mkShell {
                stdenv = pkgs.clangStdenv;

                nativeBuildInputs = with pkgs; [
                    meson
                    ninja
                ];
            };
        });
    };
}
