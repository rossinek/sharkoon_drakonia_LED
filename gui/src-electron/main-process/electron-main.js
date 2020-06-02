import { app, BrowserWindow, nativeTheme, Tray, ipcMain } from 'electron'
import path from 'path'
import { execFileSync } from 'child_process'

try {
  if (process.platform === 'win32' && nativeTheme.shouldUseDarkColors === true) {
    require('fs').unlinkSync(path.join(app.getPath('userData'), 'DevTools Extensions'))
  }
} catch (_) { }

/**
 * Set `__statics` path to static files in production;
 * The reason we are setting it here is that the path needs to be evaluated at runtime
 */
if (process.env.PROD) {
  global.__statics = path.join(__dirname, 'statics').replace(/\\/g, '\\\\')
  global.__binDriverPath = path.resolve(__dirname, '../sharkoonLED').replace(/\\/g, '\\\\')
} else {
  global.__binDriverPath = path.resolve(__dirname, '../../../bin/sharkoonLED').replace(/\\/g, '\\\\')
}

let tray
let mainWindow

const createWindow = () => {
  mainWindow = new BrowserWindow({
    width: 410,
    height: 380,
    useContentSize: true,
    show: false,
    frame: false,
    fullscreenable: false,
    resizable: false,
    transparent: true,
    webPreferences: {
      // Prevents renderer process code from not running when window is
      // hidden
      backgroundThrottling: false,
      // Change from /quasar.conf.js > electron > nodeIntegration;
      // More info: https://quasar.dev/quasar-cli/developing-electron-apps/node-integration
      // eslint-disable-next-line no-undef
      nodeIntegration: QUASAR_NODE_INTEGRATION,
      // eslint-disable-next-line no-undef
      nodeIntegrationInWorker: QUASAR_NODE_INTEGRATION
      // More info: /quasar-cli/developing-electron-apps/electron-preload-script
      // preload: path.resolve(__dirname, 'electron-preload.js')
    }
  })
  mainWindow.loadURL(process.env.APP_URL)

  // Hide the window when it loses focus
  mainWindow.on('blur', () => {
    if (!mainWindow.webContents.isDevToolsOpened()) {
      mainWindow.hide()
    }
  })
}

const getWindowPosition = () => {
  const windowBounds = mainWindow.getBounds()
  const trayBounds = tray.getBounds()
  // Center window horizontally below the tray icon
  const x = Math.round(trayBounds.x + (trayBounds.width / 2) - (windowBounds.width / 2))
  // Position window 4 pixels vertically below the tray icon
  const y = Math.round(trayBounds.y + trayBounds.height + 4)
  return { x, y }
}

const showWindow = () => {
  const position = getWindowPosition()
  mainWindow.setPosition(position.x, position.y, false)
  mainWindow.show()
  mainWindow.focus()
}

const toggleWindow = () => {
  if (mainWindow.isVisible()) {
    mainWindow.hide()
  } else {
    showWindow()
  }
}

const createTray = () => {
  tray = new Tray(path.join(__statics, 'tray-icon-16.png'))
  tray.on('right-click', toggleWindow)
  tray.on('double-click', toggleWindow)
  tray.on('click', function (event) {
    toggleWindow()

    // Show devtools when command clicked
    if (mainWindow.isVisible() && process.defaultApp && event.metaKey) {
      mainWindow.openDevTools({ mode: 'detach' })
    }
  })
}

app.dock.hide()

app.on('ready', () => {
  createTray()
  createWindow()
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

ipcMain.on('send-to-mouse', (event, args) => {
  execFileSync(global.__binDriverPath, args)
})
