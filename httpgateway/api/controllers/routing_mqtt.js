'use strict';

const THIS_BASE_PATH = process.env.THIS_BASE_PATH;
const CONTROLLERS_BASE = THIS_BASE_PATH + '/api/controllers/';
const BACKEND_BASE = THIS_BASE_PATH + '/amplify/backend/function/';

const MQTT_BROKER_URL = process.env.MQTT_BROKER_URL;

const MQTT_TARGET_FNAME = "mqtt.json";
const DEFAULT_HANDLER = "handler";

const fs = require('fs');
const mqtt = require('mqtt')

let mqtt_client;

function parse_mqtt() {
  if( !MQTT_BROKER_URL ){
    console.log('can not load mqtt');
    return;
  }

  let topic_list = [];
  mqtt_client = mqtt.connect(MQTT_BROKER_URL);
  mqtt_client.on('connect', () =>{
    // mqtt.jsonの検索
    const folders = fs.readdirSync(CONTROLLERS_BASE);
    folders.forEach(folder => {
      try {
        const stats_dir = fs.statSync(CONTROLLERS_BASE + folder);
        if (!stats_dir.isDirectory())
          return;

        const fname = CONTROLLERS_BASE + folder + "/" + MQTT_TARGET_FNAME;
        if( !fs.existsSync(fname) )
          return;
        const stats_file = fs.statSync(fname);
        if (!stats_file.isFile())
          return;

        const defs = JSON.parse(fs.readFileSync(fname).toString());

        defs.forEach(item =>{
          if (!item.enable)
            return;

          const handler = item.handler || DEFAULT_HANDLER;
          const proc = require('./' + folder)[handler];

          // topicの検索
          if( !item.topic ){
            // for startup
            const context = { mqtt: mqtt_client };
            proc(null, context);
            return;
          }

          topic_list.push({
            topic: item.topic,
            regexp: new RegExp(item.topic.replace(/#/g, '[^/]+')),
            proc: proc,
            isBase64Encoded: item.isBase64Encoded ? true : false
          } );

          mqtt_client.subscribe(item.topic, () =>{
            console.log(item.topic + " mqtt " + handler);
          });
        });

      } catch (error) {
        console.log(error);
      }
    });
  });

  mqtt_client.on('message', (topic, message) => {
    topic_list.forEach( item => {
      // topicの判別
      try{
        const result = topic.match(item.regexp);
        if(result){
          const context = { topic: topic, mqtt: mqtt_client, isBase64Encoded: item.isBase64Encoded };
          if (item.isBase64Encoded )
            item.proc(message.toString('base64'), context)
            .catch(error =>{
              console.log(error);
            });
          else
            item.proc(message.toString(), context)
            .catch(error =>{
              console.log(error);
            });
        }
      }catch(error){
        console.log(error);
      }
    });
  });
}

module.exports = parse_mqtt();
