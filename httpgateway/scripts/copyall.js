const fse = require('fs-extra');

const THIS_BASE_PATH = __dirname + "/..";

console.log("copy /public to /dist");
fse.copySync(THIS_BASE_PATH + "/public", THIS_BASE_PATH + "/dist");
