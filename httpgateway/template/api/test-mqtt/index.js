'use strict';

const HELPER_BASE = process.env.HELPER_BASE || "/opt/";

exports.handler = async (event, context) => {
  console.log(event);
  console.log(context);
  context.mqtt.publish('test', "Hello World");
};
