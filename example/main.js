const {app, BrowserWindow, ipcMain} = require('electron')
const path = require('path')

function createWindow() {
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js')
        }
    })

    win.loadFile('index.html')
}

app.allowRendererProcessReuse = false

app.whenReady().then(() => {
    createWindow()

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
})

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
})

const wallpaper = require('..')

const windows = []

const os = require("os")

function getNativeWindowHandle_Int(win) {
    let hbuf = win.getNativeWindowHandle()

    if (os.endianness() === "LE") {
        return hbuf.readInt32LE()
    } else {
        return hbuf.readInt32BE()
    }
}

ipcMain.on('attach', (event, params) => {
    const window = new BrowserWindow({
        webPreferences: {
            contextIsolation: true
        },
        width: 800,
        height: 600,
        x: 1920,
        y: 0,
        frame: false,
        title: 'Experimental-wallpaper'
    })

    const title = window.getTitle();

    const nativeHandle = window.getNativeWindowHandle();

    window.loadFile('index.html')

    console.log(nativeHandle)

    console.log(getNativeWindowHandle_Int(window))

    windows.push(window)

    try {
        wallpaper.attach(nativeHandle)
    } catch (error) {
        throw error
    }
})

ipcMain.on('detach', (event, params) => {
    if (!windows.length) {
        throw Error('No window to detach')
    }

    const window = windows[windows.length - 1];

    const nativeHandle = window.getNativeWindowHandle();

    const detach = wallpaper.detach(nativeHandle)

    console.log('detached', nativeHandle)

    windows.splice(windows.length - 1, 1)
})
