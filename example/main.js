const {
  app,
  BrowserWindow,
  ipcMain
} = require('electron')

const path = require('path')

const wallpaper = require('..')

function createWindow() {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    transparent: true,
    frame: false,
    backgroundColor: '#18307f',
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })

  win.loadFile('index.html')

  wallpaper.attach(win.getNativeWindowHandle(), 0, 0)

  // setTimeout(() => {
  //   wallpaper.setPosition(win.getNativeWindowHandle(), -1920, 0)
  // }, 5000)

  // setTimeout(() => {
  //   wallpaper.detach(win.getNativeWindowHandle())
  // }, 10000)

  console.log(wallpaper.getCoordinate(-1920, -576))
  console.log(wallpaper.getCoordinate(0, 0))
  console.log(wallpaper.getCoordinate(1920, -576))
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

const windows = []

ipcMain.on('attach', (event, params) => {
  const window = new BrowserWindow({
    webPreferences: {
      contextIsolation: true
    },
    width: 800,
    height: 600,
    backgroundColor: '#ff0000',
    // x: -1920,
    // y: 0,
    transparent: true,
    frame: false
  })

  const nativeHandle = window.getNativeWindowHandle()

  window.loadFile('index.html')

  windows.push(window)

  wallpaper.attach(nativeHandle, 0, 0)
})

ipcMain.on('detach', (event, params) => {
  if (!windows.length) {
    throw Error('No window to detach')
  }

  const window = windows[windows.length - 1]

  const nativeHandle = window.getNativeWindowHandle()

  wallpaper.detach(nativeHandle)

  console.log('detached', nativeHandle)

  // window.close()

  windows.splice(windows.length - 1, 1)
})
