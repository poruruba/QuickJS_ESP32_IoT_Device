'use strict';

const HELPER_BASE = process.env.HELPER_BASE || "/opt/";
const Response = require(HELPER_BASE + 'response');
const Redirect = require(HELPER_BASE + 'redirect');

exports.handler = async (event, context, callback) => {
	var body = JSON.parse(event.body);
	console.log(body);
	return new Response({ message: 'Hello World' });
};
