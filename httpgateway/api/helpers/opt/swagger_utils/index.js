'use strict';

//const ROUTING_NAME = "routing";

const yaml = require('yaml');
const yaml_types = require('yaml/types');

function parse_document(file){
  return yaml.parseDocument(file);
}

function append_paths(root, target, name){
  var map = target.get('paths');
  if( !map )
    return 0;

  var num = 0;
  for( var i = 0 ; i < map.items.length ; i++ ){
    if( map.items[i].value.items[0].value instanceof yaml_types.Scalar )
      continue;

    var children = map.items[i].value.items;
    for( var j = 0 ; j < children.length ; j++ ){
//      children[j].value.set("x-swagger-router-controller", ROUTING_NAME);
      children[j].value.set("operationId", name);
      children[j].value.set("x-automount", name);
      if( children[j].value.has('tags') )
        children[j].value.addIn(['tags'], name);
      else
        children[j].value.add({ key: 'tags', value: [name] });
    }

    map.items[i].comment = " automounted";
    root.addIn(['paths'], map.items[i]);
    console.log('mounted(paths): ' + map.items[i].key.value);
    num++;
  }

  return num;
}

function append_definitions(root, target, name){
  var map = target.get('definitions');
  if( !map )
    return 0;

  for( var i = 0 ; i < map.items.length ; i++ ){
    map.items[i].value.set("x-automount", name);

    map.items[i].comment = " automounted";
    root.addIn(['definitions'], map.items[i]);
    console.log('mounted(definition): ' + map.items[i].key.value);
  }

  return map.items.length;
}

function delete_paths(root){
  var map = root.get('paths');
  if( !map )
    return 0;

  var delete_target = [];

  for( var i = 0 ; i < map.items.length ; i++ ){
    if( map.items[i].value.items[0].value instanceof yaml_types.Scalar )
      continue;

    var children = map.items[i].value.items;
    for( var j = 0 ; j < children.length ; j++ ){
      if( children[j].value.get("x-automount") ){
        delete_target.push(map.items[i].key.value);
        break;
      }
    }
  }

  for( var i = 0 ; i < delete_target.length ; i++ )
    map.delete(delete_target[i]);
  
  return delete_target.length;
}

function delete_definitions(root){
  var map = root.get('definitions');
  if( !map )
    return 0;

  var delete_target = [];
  for( var i = 0 ; i < map.items.length ; i++ ){
    if( map.items[i].value.get("x-automount") )
      delete_target.push(map.items[i].key.value);
  }

  for( var i = 0 ; i < delete_target.length ; i++ )
    map.delete(delete_target[i]);

  return delete_target.length;
}

module.exports = {
  parse_document,
  append_paths,
  append_definitions,
  delete_paths,
  delete_definitions,
};
