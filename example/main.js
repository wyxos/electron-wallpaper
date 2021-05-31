const {
  app,
  BrowserWindow,
  ipcMain
} = require('electron')
const path = require('path')

function createWindow () {
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
    webPreferences: {
      contextIsolation: true
    },
    width: 800,
    height: 600,
    x: 1920,
    y: 0,
    transparent: true,
    frame: false
  })

  const nativeHandle = window.getNativeWindowHandle()

  window.loadFile('index.html')

  windows.push(window)

  wallpaper.attach(nativeHandle, 1920, 0)
})

ipcMain.on('detach', (event, params) => {
  if (!windows.length) {
    throw Error('No window to detach')
  }

  const window = windows[windows.length - 1]

  const nativeHandle = window.getNativeWindowHandle()

  wallpaper.detach(nativeHandle)

  console.log('detached', nativeHandle)

  window.close()

  windows.splice(windows.length - 1, 1)
})
