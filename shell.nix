with import <nixpkgs> {};
stdenv.mkDerivation {
        name = "env";
        nativeBuildInputs = [ 
                pkg-config 
                gdb 
        ];
        buildInputs = [
                sdl3 
                sdl3-ttf
        ];
}
