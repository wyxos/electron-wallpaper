{
    "targets" : [
        {
            "target_name": "ElectronWallpaper",
            "sources": [
                "src/main.cpp",
                "src/Samples/wallpaper.h",
                "src/Samples/wallpaper.cpp"
            ],
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'libraries': [],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
        },
    ]
}
