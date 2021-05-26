const electron = require('electron')

window.addEventListener('attach', function(){
    console.log("triggered attach")

    electron.ipcRenderer.send('attach')
})

window.addEventListener('detach', function(){
    console.log("triggered detach")

    electron.ipcRenderer.send('detach')
})