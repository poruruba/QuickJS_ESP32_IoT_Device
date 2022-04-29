var hashs = {};
var searchs = {};

function proc_load() {
  hashs = parse_url_vars(location.hash);
  searchs = parse_url_vars(location.search);
}

function parse_url_vars(param) {
  if( param.startsWith('#') || param.startsWith('?') )
    param = param.substr(1);
  var searchParams = new URLSearchParams(param);
  var vars = {};
  for (let p of searchParams)
    vars[p[0]] = p[1];

  return vars;
}

function vue_add_data(options, datas) {
  for (var data in datas) {
    options.data[data] = datas[data];
  }
}
function vue_add_methods(options, funcs) {
  for (var func in funcs) {
    options.methods[func] = funcs[func];
  }
}
function vue_add_computed(options, funcs) {
  for (var func in funcs) {
    options.computed[func] = funcs[func];
  }
}
function vue_add_components(options, components) {
  if (!options.components)
    options.components = {};
  for (var component in components) {
    options.components[component] = components[component];
  }
}
function vue_add_component(options, name, component) {
  if (!options.components)
    options.components = {};
  options.components[name] = component;
}

function vue_add_global_components(components) {
  for (var component in components) {
    Vue.component(component, components[component]);
  }
}
function vue_add_global_component(name, component) {
  Vue.component(name, component);
}

function loader_loaded(){
  const element = document.getElementById("loader-background");
  if( element )
    element.classList.add('loader-loaded');
}

function do_post(url, body) {
  const headers = new Headers({ "Content-Type": "application/json; charset=utf-8" });

  return fetch(url, {
    method: 'POST',
    body: JSON.stringify(body),
    headers: headers
  })
  .then((response) => {
    if (!response.ok)
      throw 'status is not 200';
    return response.json();
//    return response.text();
//    return response.blob();
//    return response.arrayBuffer();
  });
}

function do_post_urlencoded(url, params) {
  const headers = new Headers({ 'Content-Type': 'application/x-www-form-urlencoded' });
  const body = new URLSearchParams(params);

  return fetch(url, {
    method: 'POST',
    body: body,
    headers: headers
  })
  .then((response) => {
    if (!response.ok)
      throw 'status is not 200';
    return response.json();
//    return response.text();
//    return response.blob();
//    return response.arrayBuffer();
  })
}

function do_post_formdata(url, params) {
  const body = Object.entries(params).reduce((l, [k, v]) => { l.append(k, v); return l; }, new FormData());

  return fetch(url, {
    method: 'POST',
    body: body,
  })
  .then((response) => {
    if (!response.ok)
      throw 'status is not 200';
    return response.json();
//    return response.text();
//    return response.blob();
//    return response.arrayBuffer();
  });
}

function do_get(url, qs) {
  const params = new URLSearchParams(qs);

  var params_str = params.toString();
  var postfix = (params_str == "") ? "" : ((url.indexOf('?') >= 0) ? ('&' + params_str) : ('?' + params_str));
  return fetch(url + postfix, {
    method: 'GET',
  })
  .then((response) => {
    if (!response.ok)
      throw 'status is not 200';
    return response.json();
//    return response.text();
//    return response.blob();
//    return response.arrayBuffer();
  });
}
