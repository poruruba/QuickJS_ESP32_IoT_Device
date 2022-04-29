'use strict';

const HELPER_BASE = process.env.HELPER_BASE || "/opt/";
const Response = require(HELPER_BASE + 'response');
const TextResponse = require(HELPER_BASE + 'textresponse');
const BinResponse = require(HELPER_BASE + 'binresponse');

const { URLSearchParams } = require('url');
const fetch = require('node-fetch');
const Headers = fetch.Headers;

const HTTP_TYPE_MASK = 0xF;

const HTTP_RESP_SHIFT   = 0;
const HTTP_RESP_NONE    = 0x0;
const HTTP_RESP_TEXT    = 0x1;
const HTTP_RESP_JSON    = 0x2;
const HTTP_RESP_BINARY  = 0x3;

const HTTP_METHOD_SHIFT   = 4;
const HTTP_METHOD_GET    = 0x0;
const HTTP_METHOD_POST   = 0x1;

const HTTP_CONTENT_SHIFT   = 8;
const HTTP_CONTENT_NONE    = 0x0;
const HTTP_CONTENT_JSON    = 0x1;
const HTTP_CONTENT_URLENCODE    = 0x2;
const HTTP_CONTENT_FORMDATA    = 0x3;

exports.handler = async (event, context, callback) => {
	switch(event.path){
		case '/httpgateway-call':{
			var body = JSON.parse(event.body);
			console.log("type=" + body.method_type, body.url, body.params, body.headers);
			var result;
			var method = (body.method_type >> HTTP_METHOD_SHIFT) & HTTP_TYPE_MASK;
			var response_type = (body.method_type >> HTTP_RESP_SHIFT) & HTTP_TYPE_MASK;
			var content_type = (body.method_type >> HTTP_CONTENT_SHIFT) & HTTP_TYPE_MASK;
			switch(method){
				case HTTP_METHOD_POST:{
					switch(content_type){
						case HTTP_CONTENT_URLENCODE:{
							result = await do_post_urlencoded(body.url, body.params, body.headers, response_type);
							break;
						}
						case HTTP_CONTENT_FORMDATA:{
							result = await do_post_formdata(body.url, body.params, body.headers, response_type);
							break;
						}
						default:{
							result = await do_post(body.url, body.params, body.headers, response_type);
							break;
						}
					}
					break;
				}
				case HTTP_METHOD_GET:{
					result = await do_get(body.url, body.params, body.headers, response_type);
					break;
				}
				default:
					throw "unknown method";
			}

			switch(response_type){
				case HTTP_RESP_JSON: return new Response(result);
				case HTTP_RESP_BINARY: return new TextResponse("text/plain", Buffer.from(result).toString('base64'));
				default: return new TextResponse("text/plain", result);
			}
		}
	}
};

function do_post(url, body, header, response_type = HTTP_RESP_JSON) {
	const headers = new Headers(header);
	headers.append("Content-Type", "application/json");

	return fetch(url, {
		method: 'POST',
		body: JSON.stringify(body),
		headers: headers
	})
		.then((response) => {
			if (!response.ok)
				throw 'status is not 200';

			switch(response_type){
				case HTTP_RESP_JSON: return response.json();
				case HTTP_RESP_BINARY: return response.arrayBuffer();
				default: return response.text();
			}
		});
}

function do_post_urlencoded(url, params, header, response_type = HTTP_RESP_JSON){
	var data = new URLSearchParams();
	for( var name in params )
			data.append(name, params[name]);

	const headers = new Headers(header);
	headers.append("Content-Type", 'application/x-www-form-urlencoded');
	
	return fetch(url, {
			method : 'POST',
			body : data,
			headers: headers
	})
	.then((response) => {
		if( response.status != 200 )
			throw 'status is not 200';

			switch(response_type){
				case HTTP_RESP_JSON: return response.json();
				case HTTP_RESP_BINARY: return response.arrayBuffer();
				default: return response.text();
			}
	});
}

function do_post_formdata(url, params, header, response_type = HTTP_RESP_JSON){
	var body = Object.entries(params).reduce( (l, [k,v])=> { l.append(k, v); return l; }, new FormData());
	const headers = new Headers(header);

	return fetch(url, {
			method : 'POST',
			body: body,
			headers: headers,
	})
	.then((response) => {
		if( !response.ok )
			throw 'status is not 200';

			switch(response_type){
				case HTTP_RESP_JSON: return response.json();
				case HTTP_RESP_BINARY: return response.arrayBuffer();
				default: return response.text();
			}
	});
}

function do_get(url, qs, header, response_type = HTTP_RESP_JSON) {
	const params = new URLSearchParams(qs);
	const headers = new Headers(header);

  var params_str = params.toString();
  var postfix = (params_str == "") ? "" : ((url.indexOf('?') >= 0) ? ('&' + params_str) : ('?' + params_str));

  return fetch(url + postfix, {
		method: 'GET',
		headers: headers
	})
		.then((response) => {
			if (!response.ok)
				throw 'status is not 200';

			switch(response_type){
				case HTTP_RESP_JSON: return response.json();
				case HTTP_RESP_BINARY: return response.arrayBuffer();
				default: return response.text();
			}
		});
}