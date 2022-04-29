'use strict';

async function parse(parser, event) {
  var body = await parser.parse(event);

  if (body.files) {
    event.files = {};
    for (var i = 0; i < body.files.length; i++) {
      var file = {};
      file.buffer = body.files[i].content;
      file.encoding = body.files[i].encoding;
      file.fieldname = body.files[i].fieldname;
      file.mimetype = body.files[i].contentType;
      file.originalname = body.files[i].filename;
      file.size = body.files[i].content.length;
      event.files[file.fieldname] = [file];
    }
    delete body.files;
  }

  return body;
}

module.exports = {
  parse,
};
