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

ipcMain.on('attach', (event, params) => {
    const window = new BrowserWindow({
        width: 800,
        height: 600,
        x: 1920,
        y: 0,
        frame: false,
        title: 'Experimental-wallpaper'
    })

    window.loadFile('index.html')

    const title = window.getTitle();

    const attach = wallpaper.attach(title)

    console.log(attach)

    windows.push(title)
})

ipcMain.on('detach', (event, params) => {
    if(!windows.length){
        throw Error('No window to detach')
    }

    const title = windows[windows.length - 1];

    const detach = wallpaper.detach(title)

    console.log('detached', title)

    windows.splice(windows.length - 1, 1)
})
