'use strict';

const fs = require('fs').promises;

async function read_json(fname, initial) {
  try {
    var result = await fs.readFile(fname);
    return JSON.parse(result);
  } catch (error) {
    return initial;
  }
}

async function write_json(fname, json) {
  await fs.writeFile(fname, JSON.stringify(json, null, '\t'));
}

module.exports = {
  read_json,
  write_json,
};