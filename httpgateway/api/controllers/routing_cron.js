'use strict';

const THIS_BASE_PATH = process.env.THIS_BASE_PATH;
const CONTROLLERS_BASE = THIS_BASE_PATH + '/api/controllers/';
const BACKEND_BASE = THIS_BASE_PATH + '/amplify/backend/function/';

const CRON_TARGET_FNAME = "cron.json";
const DEFAULT_HANDLER = "handler";

const fs = require('fs');
const cron = require('node-cron');

function parse_cron() {
  // cron.jsonの検索
  const folders = fs.readdirSync(CONTROLLERS_BASE);
  folders.forEach(folder => {
    try {
      const stats_dir = fs.statSync(CONTROLLERS_BASE + folder);
      if (!stats_dir.isDirectory())
        return;
    
      const fname = CONTROLLERS_BASE + folder + "/" + CRON_TARGET_FNAME;
      if (!fs.existsSync(fname))
        return;
      const stats_file = fs.statSync(fname);
      if (!stats_file.isFile())
        return;

      // cronの登録
      const defs = JSON.parse(fs.readFileSync(fname).toString());
      parse_cron_json(defs, CONTROLLERS_BASE + folder, folder);
    } catch (error) {
      console.log(error);
    }
  });

  if (fs.existsSync(BACKEND_BASE)){
	  const stats_folder2 = fs.statSync(BACKEND_BASE);
	  if (stats_folder2.isDirectory()){
		  const folders2 = fs.readdirSync(BACKEND_BASE);
		  folders2.forEach(folder => {
		    try {
		    const stats_dir = fs.statSync(BACKEND_BASE + folder);
		    if (!stats_dir.isDirectory())
		      return;

		      const fname = BACKEND_BASE + folder + "/src/" + CRON_TARGET_FNAME;
		      if (!fs.existsSync(fname))
		        return;
		      const stats_file = fs.statSync(fname);
		      if (!stats_file.isFile())
		        return;

		      // cronの登録
		      const defs = JSON.parse(fs.readFileSync(fname).toString());
		      parse_cron_json(defs, BACKEND_BASE + folder, folder);
		    } catch (error) {
		      console.log(error);
		    }
		  });
	  }
  }
}

function parse_cron_json(defs, folder, folder_name) {
  defs.forEach(item => {
    if (!item.enable )
      return;
      
    const handler = item.handler || DEFAULT_HANDLER;
    const proc = require(folder)[handler];

    cron.schedule(item.schedule, () => proc(item.param));
    console.log(item.schedule + " cron " + folder_name + ' ' + handler);
  });
}

module.exports = parse_cron();
