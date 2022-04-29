'use strict';

const THIS_BASE_PATH = process.env.THIS_BASE_PATH;

const CONTROLLERS_BASE = THIS_BASE_PATH + '/api/controllers/';
const BACKEND_BASE = THIS_BASE_PATH + '/amplify/backend/function/';
const SWAGGER_DEFAULT_BASE = THIS_BASE_PATH + '/api/swagger/';
const TARGET_FNAME = "swagger.yaml";

const express = require('express');
const router = express.Router();

const fs = require('fs');
const yaml = require('yaml');
const multer = require('multer');
const jwt_decode = require('jwt-decode');

let swagger_basePath = '/';

const fname = SWAGGER_DEFAULT_BASE + TARGET_FNAME;

function parse_swagger_yaml(swagger, folder, folder_name)
{
  // paths配下のみ参照
  const paths = swagger.get('paths');
  paths.items.forEach(docPath =>{
    docPath.value.items.forEach(docMethod =>{
      if (docMethod.key.value != 'get' && docMethod.key.value != 'post' && docMethod.key.value != 'head')
        return;

      let options = parse_swagger_method(docMethod);
      if (!options.operationId)
        options.operationId = folder_name;

      const path = docPath.key.value;
      console.log(path, options.method, options.handler, JSON.stringify(options));

      let postprocess;
      let nextfunc;
      if( options.func_type == "express"){
        // x-functype: express の場合
        nextfunc = require(folder)[options.handler];
      }else
      if( options.func_type == 'empty' ){
        // x-functype: empty の場合
        nextfunc = (req, res) => res.json({});
      }else{
        // x-functype: normal|alexa|lambda の場合
        postprocess = require(folder)[options.handler];
        nextfunc = routing;
      }

      switch (options.method){
        case 'get': {
          router.get(path, preprocess(options, postprocess), nextfunc);
          break;
        }
        case 'post': {
          router.post(path, preprocess(options, postprocess), nextfunc);
          break;
        }
        case 'head': {
          router.head(path, preprocess(options, postprocess), nextfunc);
          break;
        }
      }
    });
  });
}

if( fs.existsSync(fname) ){
  const stats_file = fs.statSync(fname);
  if( stats_file.isFile() ){
    try{
      const swagger = yaml.parseDocument(fs.readFileSync(fname, 'utf-8'));
      const base_path = swagger.get('basePath');
      if( base_path )
        swagger_basePath = base_path;

      // paths配下のみ参照
      const paths = swagger.get('paths');
      paths.items.forEach(docPath =>{
        docPath.value.items.forEach(docMethod =>{
          if( docMethod.key.value != 'get' && docMethod.key.value != 'post' && docMethod.key.value != 'head' )
            return;

          let options = parse_swagger_method(docMethod);
          if( !options.operationId )
            throw "operationId is not defined";

          const path = docPath.key.value;
          console.log(path, options.method, options.handler, JSON.stringify(options));

          let postprocess;
          let nextfunc;
          if( options.func_type == "express"){
            // x-functype: express の場合
            nextfunc = require(CONTROLLERS_BASE + options.operationId)[options.handler];
          }else
          if( options.func_type == 'empty' ){
            // x-functype: empty の場合
            nextfunc = (req, res) => res.json({});
          }else{
            // x-functype: normal|alexa|lambda の場合
            postprocess = require(CONTROLLERS_BASE + options.operationId)[options.handler];
            nextfunc = routing;
          }
  
          switch(options.method){
            case 'get': {
              router.get(path, preprocess(options, postprocess), nextfunc);
              break;
            }
            case 'post': {
              router.post(path, preprocess(options, postprocess), nextfunc);
              break;
            }
            case 'head': {
              router.head(path, preprocess(options, postprocess), nextfunc);
              break;
            }
          }
        });
      });
    }catch(error){
      console.error(error);
    }
  }
}

// swagger.yamlの検索
const folders = fs.readdirSync(CONTROLLERS_BASE);
folders.forEach(folder => {
  try{
    const stats_dir = fs.statSync(CONTROLLERS_BASE + folder);
    if( !stats_dir.isDirectory() )
        return;

    const fname = CONTROLLERS_BASE + folder + "/" + TARGET_FNAME;
    if( !fs.existsSync(fname) )
      return;
    const stats_file = fs.statSync(fname);
    if( !stats_file.isFile() )
      return;

    // swagger.yamlの解析
    const swagger = yaml.parseDocument(fs.readFileSync(fname, 'utf-8'));
    parse_swagger_yaml(swagger, CONTROLLERS_BASE + folder, folder);
  }catch(error){
    console.log(error);
    return;
  }
});

if( fs.existsSync(BACKEND_BASE) ){
	const stats_folder2 = fs.statSync(BACKEND_BASE);
	if( stats_folder2.isDirectory() ){
	  const folders2 = fs.readdirSync(BACKEND_BASE);
	  folders2.forEach(folder => {
	    try{
        const stats_dir = fs.statSync(BACKEND_BASE + folder);
        if( !stats_dir.isDirectory() )
            return;

        const fname = BACKEND_BASE + folder + "/src/" + TARGET_FNAME;
	      if( !fs.existsSync(fname) )
	        return;
	      const stats_file = fs.statSync(fname);
	      if( !stats_file.isFile() )
	        return;

	      // swagger.yamlの解析
	      const swagger = yaml.parseDocument(fs.readFileSync(fname, 'utf-8'));
	      parse_swagger_yaml(swagger, BACKEND_BASE + folder + "/src", folder);
	    }catch(error){
	      console.log(error);
	      return;
	    }
	  });
	}
}

function parse_swagger_method(docMethod) {
  // デフォルト値
  const options = {
    operationId: null,
    func_type: 'normal',
    content_type: 'application/json',
    handler: 'handler',
    files: []
  };

  // methodの取得
  options.method = docMethod.key.value;

  // オプションタグ: operationId
  // operationId: 任意
  const docOperationId = docMethod.value.items.filter(item => item.key.value == 'operationId');
  if (docOperationId.length == 1)
    options.operationId = docOperationId[0].value.value;

  // オプションタグ: x-hanndler
  // x-hanndler: 任意
  const docHandler = docMethod.value.items.filter(item => item.key.value == 'x-handler');
  if (docHandler.length == 1)
    options.handler = docHandler[0].value.value;

  // オプションタグ: security
  // security:
  // - basicAuth: []
  // - tokenAuth: []
  // - apikeyAuth: []
  // - jwtAuth: []
  const docSecurity = docMethod.value.items.filter(item => item.key.value == 'security');
  if (docSecurity.length == 1 && docSecurity[0].value.items.length == 1 && docSecurity[0].value.items[0].items.length == 1)
    options.security = docSecurity[0].value.items[0].items[0].key.value;

  // オプションタグ: x-functype
  // x-functype: (express|empty|normal|alexa|lambda)
  const docFuncType = docMethod.value.items.filter(item => item.key.value == 'x-functype');
  if (docFuncType.length == 1)
    options.func_type = docFuncType[0].value.value;

  // オプションタグ: consumes
  const docConsumes = docMethod.value.items.filter(item => item.key.value == 'consumes');
  if (docConsumes.length == 1 && docConsumes[0].value.items.length == 1)
    options.content_type = docConsumes[0].value.items[0].value;

  // file(multipart/form-data)の処理
  // parameters:
  // - in: formData
  //   type: file
  //   name: 任意
  if (options.content_type == 'multipart/form-data') {
    const parameters = docMethod.value.items.filter(item => item.key.value == 'parameters');
    parameters.forEach(parameter => {
      parameter.value.items.forEach(item2 => {
        const item_in = item2.items.filter(item => item.key.value == 'in' && item.value.value == 'formData');
        if (item_in.length != 1)
          return;
        const item_type = item2.items.filter(item => item.key.value == 'type' && item.value.value == 'file');
        if (item_type.length != 1)
          return;
        const item_name = item2.items.filter(item => item.key.value == 'name');
        if (item_name.length != 1)
          return;

        options.files.push({ name: item_name[0].value.value });
      });
    });
  }

  return options;
}

// x-functype: 前処理
function preprocess(options, postprocess){
  return function(req, res, next){
    req.swagger = {
      operation: {
        operationId: options.operationId
      }
    }
    req.postprocess = postprocess;
    res.func_type = options.func_type;

    // securityの処理
    switch( options.security ){
      case 'tokenAuth':{
        if( req.headers.authorization ){
          const decoded = jwt_decode(req.headers.authorization);
          req.requestContext = {
            authorizer : {
              claims : decoded
            }
          };
        }
        break;
      }
      case 'basicAuth': {
        if( req.headers.authorization ){
          let basic = req.headers.authorization.trim();
          if(basic.toLowerCase().startsWith('basic '))
            basic = basic.slice(6).trim();
  
          const buf = Buffer.from(basic, 'base64');
          const ascii = buf.toString('ascii');
  
          req.requestContext = {
            basicAuth : {
              basic : ascii.split(':')
            }
          };
        }
        break;
      }
      case 'jwtAuth': {
        if( req.headers.authorization ){
          const decoded = jwt_decode(req.headers.authorization);
          req.requestContext = {
            jwtAuth : {
              claims : decoded
            }
          };
          const claims = {
            claims : decode,
            issuer: decoded.iss,
            id: decoded.sub,
            email: decoded.email
          };
          const buffer = Buffer.from(JSON.stringify(claims));
          req.headers['x-endpoint-api-userinfo'] = buffer.toString('base64');
        }
        break;
      }
      case 'apikeyAuth': {
        const apikey = req.headers["x-api-key"];
        if( apikey ){
          req.requestContext = {
            apikeyAuth : {
              apikey : apikey
            }
          };
        }
        break;
      }
    }

    // file(multipart/form-data)の処理    
    if( options.content_type == 'multipart/form-data'){
      let upload;
      if( options.files && options.files.length > 0 ){
        upload = multer({ storage: multer.memoryStorage() }).fields(options.files);
      }else{
        upload = multer().none();
      }
      upload(req, res, function(err){
        if(err)
          throw err;
        next();
      });
    }else{
      next();
    }
  }
}

// x-functype: normal の場合の後処理
function routing(req, res) {
//  console.log(req);

  const operationId = req.swagger.operation.operationId;
  console.log('[' + req.path + ' calling]');

  try{
      let event;
      const func = req.postprocess;
      if( res.func_type == 'express'){
        func(req, res);
        return;
      }else
      if( res.func_type == 'normal' ){
          event = {
              headers: req.headers,
              body: JSON.stringify(req.body),
              path: req.path,
              httpMethod: req.method,
              queryStringParameters: req.query,
              stage: req.baseUrl ? req.baseUrl : '/',
              Host: req.hostname,
              requestContext: ( req.requestContext ) ? req.requestContext : {},
              files: req.files,
          };
      }else
      if( res.func_type == 'alexa' ){
          event = req.body;
      }else
      if( res.func_type == 'lambda' ){
          event = req.body.event;
      }else{
          console.log('can not found operationId: ' + operationId);
          return_error(res, new Error('can not found operationId'));
          return;
      }
      res.returned = false;

//        console.log(event);

      const context = {
          succeed: (msg) => {
              console.log('succeed called');
              return_response(res, msg);
          },
          fail: (error) => {
              console.log('failed called');
              return_error(res, error);
          },
          req: req,
          swagger: req.swagger
      };

      const task = func(event, context, (error, response) =>{
          console.log('callback called');
          if( error )
              return_error(res, error);
          else
              return_response(res, response);
      });
      if( task instanceof Promise || (task && typeof task.then === 'function') ){
          return task.then(ret =>{
              if( ret ){
                  console.log('promise is called');
                  return_response(res, ret);
              }else{
                  console.log('promise return undefined');
                  return_none(res);
              }
          })
          .catch(err =>{
              console.log('error throwed: ' + err);
              return_error(res, err);
          });
      }else{
          console.log('return called');
//            return_none(res);
      }
  }catch(err){
      console.log('error throwed: ' + err);
      return_error(res, err);
  }
}

function return_none(res){
    if( res.returned )
        return;
    else
        res.returned = true;

    res.type('application/json');

    if(res.func_type == 'lambda'){
        res.json({ body: null });
    }else{
        res.json({});
    }
}

function return_error(res, err){
    if( res.returned )
        return;
    else
        res.returned = true;

    res.status(500);
    res.json({ errorMessage: err.toString() });
}

function return_response(res, ret){
    if( res.returned )
        return;
    else
        res.returned = true;

    if( ret.statusCode )
        res.status(ret.statusCode);
    for( let key in ret.headers )
        res.set(key, ret.headers[key]);

//    console.log(ret.body);

    if (!res.get('Content-Type'))
        res.type('application/json');

    if( ret.isBase64Encoded ){
        const bin = Buffer.from(ret.body, 'base64')
        res.send(bin);
    }else{
        if( res.func_type == 'alexa'){
            res.json(ret);
        }else if( res.func_type == 'lambda'){
            res.json({ body: ret });
        }else{
            if( ret.body || ret.body == '' )
                res.send(ret.body);
            else
                res.json({});
        }
    }
}

module.exports = {
  router: router,
  basePath: swagger_basePath
};
