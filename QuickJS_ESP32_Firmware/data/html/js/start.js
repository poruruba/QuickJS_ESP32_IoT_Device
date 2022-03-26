'use strict';

var base_url = "";
var arduino = new Arduino(base_url);

var vue_options = {
    el: "#top",
    mixins: [mixins_bootstrap],
    data: {
    	input_text: "",
        upload_js: null,
    	downloaded_js: "",
        downloaded_fname: null,
        upload_mode: "main",
        target_module: "",
        module_name: "",
        module_list: [],
        base_url: base_url,
        ipaddress: "",
        macaddress: "",

        rtc_date: {},
        rtc_time: {},

        arduino: arduino,
        Wire: new Wire(this.arduino),
        Wire1: new Wire(this.arduino, "Wire1"),
        Gpio: new Gpio(this.arduino),
        Ledc: new Ledc(this.arduino),
        Pixels: new Pixels(this.arduino),
        Sd: new Sd(this.arduino),
        Rtc: new Rtc(this.arduino),
        
        wires: [],

        wire_sda: 32,
        wire_scl: 33,
        wire_address: 0,
        wire_index: 0,
        wire_write_value: "",
        wire_mode: "read",
        wire_read_len: 0,
        wire_read_value: "",
        wire_devices: [],

        gpio_pin: 10,
        gpio_pin_mode: 1,
        gpio_value: 0,
        gpio_analog_value: 0,

        ledc_channel: 1,
        ledc_setup_freq: 50,
        ledc_setup_resolution: 16,
        ledc_pin: 26,
        ledc_duty: 1500,
        ledc_freq: 0,
        ledc_note: 0,
        ledc_octave: 0,

        pixels_pin: 26,
        pixels_index: 0,
        pixels_color: "#000000",

        sd_filename: "",
        sd_exists_result: null,
        sd_mkdir_result: null,
        sd_remove_result: null,
        sd_rmdir_result: null,
        sd_size_result: null,
        sd_readText_result: null,
        sd_writeText_input: null,
        sd_writeText_result: null,
        sd_appendText_result: null,
        sd_file_list: [],
        sd_target_file: "",

        esp32_putText_input: null,
        esp32_millis_result: null,
    },
    computed: {
    },
    methods: {
        // Rtc
        rtc_set_time: async function(){
            try{
                var result = await this.Rtc.setTime(this.rtc_time);
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        rtc_get_time: async function(){
            try{
                var result = await this.Rtc.getTime();
                console.log(result);
                this.rtc_time = result;
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        rtc_set_date: async function(){
            try{
                var result = await this.Rtc.setDate(this.rtc_date);
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        rtc_get_date: async function(){
            try{
                var result = await this.Rtc.getDate();
                console.log(result);
                this.rtc_date = result;
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        // SD
        sd_exists: async function(){
            try{
                var result = await this.Sd.exists(this.sd_filename);
                this.sd_exists_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_mkdir: async function(){
            try{
                var result = await this.Sd.mkdir(this.sd_filename);
                this.sd_mkdir_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_remove: async function(){
            try{
                var result = await this.Sd.remove(this.sd_target_file);
                this.sd_remove_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_rmdir: async function(){
            try{
                var result = await this.Sd.rmdir(this.sd_target_file);
                this.sd_rmdir_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_size: async function(){
            try{
                var result = await this.Sd.size(this.sd_target_file);
                this.sd_size_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_readText: async function(){
            try{
                var result = await this.Sd.readText(this.sd_target_file);
                this.sd_readText_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_writeText: async function(){
            try{
                var result = await this.Sd.writeText(this.sd_filename, this.sd_writeText_input);
                this.sd_writeText_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_appendText: async function(){
            try{
                var result = await this.Sd.appendText(this.sd_filename, this.sd_writeText_input);
                this.sd_appendText_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        sd_list: async function(){
            try{
                var result = await this.Sd.list(this.sd_filename);
                this.sd_file_list = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },

        // ESP32
        esp32_restart: async function(){
            try{
                await this.arduino.restart();
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        esp32_reboot: async function(){
            try{
                await this.arduino.reboot();
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        esp32_putText: async function(){
            try{
                await this.arduino.putText(this.esp32_putText_input);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        esp32_millis: async function(){
            try{
                var result = await this.arduino.millis();
                this.esp32_millis_result = result;
                console.log(result);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        esp32_get_ipaddress_macaddress: async function(){
            try{
                var result = await this.arduino.getIpAddress();
                console.log(result);
                this.ipaddress = ((result >> 24) & 0xff) + "." + ((result >> 16) & 0xff) + "." + ((result >> 8) & 0xff) + "." + (result & 0xff);
                result = await this.arduino.getMacAddress();
                console.log(result);
                this.macaddress = this.to2h(result[0]) + ":" + this.to2h(result[1]) + ":" + this.to2h(result[2]) 
                                    + ":" + this.to2h(result[3]) + ":" + this.to2h(result[4]) + ":" + this.to2h(result[5]);
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        to2h(d){
            return ("00" + d.toString(16)).slice(-2);
        },

        // Wire
        wire_begin: async function(){
            try{
                await this.wires[this.wire_index].begin(this.wire_sda, this.wire_scl);
                this.toast_show("Wire", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        wire_scanDevice: async function(){
            try{
                this.progress_open();
                this.wire_devices = [];
                for(var address = 1; address < 127; address++ ){
                    await this.wires[this.wire_index].beginTransmission(address);
                    var error = await this.wires[this.wire_index].endTransmission();
                    if( error == 0 )
                        this.wire_devices.push(address);
                }
                this.toast_show("Wire", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }finally{
                this.progress_close();
            }
        },
        wire_execute: async function(){
            try{
                switch(this.wire_mode){
                    case 'read':{
                        await this.wire.beginTransmission(this.wire_address);
                        await this.wire.requestFrom(this.wire_address, this.wire_read_len);
                        var read_value = await this.wire.read(this.wire_read_len);
                        this.wire_read_data = read_value.map(item => String(item)).join(", ");
                        break;
                    }
                    case 'write':{
                        var write_value = this.wire_write_value.split(',').map(item => parseInt(item));
                        await this.wire.beginTransmission(this.wire_address);
                        await this.wire.write(write_value);
                        if( await this.wire.endTransmission() != 0 ){
                            this.toast_show("Wire", "失敗しました。", "warn");
                            return;
                        }
                            
                        break;
                    }
                    case 'write_read':{
                        var write_value = this.wire_write_value.split(',').map(item => parseInt(item));
                        await this.wire.beginTransmission(this.wire_address);
                        await this.wire.write(write_value);
                        if( await this.wire.endTransmission() != 0 ){
                            this.toast_show("Wire", "失敗しました。", "warn");
                            return;
                        }
                        await this.wire.requestFrom(this.wire_address, this.wire_read_len);
                        var read_value = await this.wire.read(this.wire_read_len);
                        this.wire_read_data = read_value.map(item => String(item)).join(", ");

                        break;
                    }
                }
            }catch(error){
                console.error(error);
                alert(error);
            }
        },

        // GPIO
        gpio_pinMode: async function(){
            try{
                await this.Gpio.pinMode(this.gpio_pin, this.gpio_pin_mode);
                this.toast_show("Gpio", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        gpio_digitalWrite: async function(){
            try{
                await this.Gpio.digitalWrite(this.gpio_pin, this.gpio_value);
                this.toast_show("Gpio", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        gpio_digitalRead: async function(){
            try{
                this.gpio_value = await this.Gpio.digitalRead(this.gpio_pin);
                this.toast_show("Gpio", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        gpio_analogRead: async function(){
            try{
                this.gpio_analog_value = await this.Gpio.analogRead(this.gpio_pin);
                this.toast_show("Gpio", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },

        // Ledc
        ledc_setup: async function(){
            try{
                await this.Ledc.setup(this.ledc_channel, this.ledc_setup_freq, this.ledc_setup_resolution);
                this.toast_show("Ledc", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        ledc_attachPin: async function(){
            try{
                await this.Ledc.attachPin(this.ledc_pin, this.ledc_channel);
                this.toast_show("Ledc", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        ledc_detachPin: async function(){
            try{
                await this.Ledc.detachPin(this.ledc_pin, this.ledc_channel);
                this.toast_show("Ledc", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        ledc_write: async function(){
            try{
                await this.Ledc.write(this.ledc_channel, this.ledc_duty);
                this.toast_show("Ledc", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        ledc_writeTone: async function(){
            try{
                await this.Ledc.writeTone(this.ledc_channel, this.ledc_freq);
                this.toast_show("Ledc", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        ledc_writeNote: async function(){
            try{
                await this.Ledc.writeNote(this.ledc_channel, this.ledc_note, this.ledc_octave);
                this.toast_show("Ledc", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },

        // Pixels
        pixels_begin: async function(){
            try{
                await this.Pixels.begin(this.pixels_pin);
                this.toast_show("Pixels", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        pixels_setOnoff: async function(onoff){
            try{
                await this.Pixels.setOnoff(onoff);
                this.toast_show("Pixels", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        pixels_clear: async function(){
            try{
                await this.Pixels.clear();
                this.toast_show("Pixels", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        pixels_setPixelColor: async function(){
            try{
                await this.Pixels.setPixelColor(this.pixels_index, this.rgb2num(this.pixels_color));
                this.toast_show("Pixels", "成功しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        rgb2num: function (rgb, sel) {
            if (sel == 'r')
              return parseInt(rgb.slice(1, 3), 16);
            else if (sel == 'g')
              return parseInt(rgb.slice(3, 5), 16);
            else if (sel == 'b')
              return parseInt(rgb.slice(5, 7), 16);
            else
              return parseInt(rgb.slice(1, 7), 16);
        },
        
        seturl: function(){
            var url = (this.base_url.endsWith('/')) ? this.base_url.slice(0, -1) : this.base_url;
            this.arduino.set_baseUrl(url);
        },
		read_file: function(files){
			if (files.length <= 0) {
				return;
			}
			var file = files[0];
            const reader = new FileReader();
            reader.onload = () => {
              this.upload_js = reader.result;
              this.dialog_open('#upload_js_dialog');
            };        
            reader.readAsText(file);
        },
		copy_to_texearea: function(){
			this.input_text = this.downloaded_js;
            if( !this.downloaded_fname ){
                this.upload_mode = "main.js"
            this.upload_mode = "main";
            }else{
                this.module_name = this.downloaded_fname;
                this.upload_mode = "module";
            }
            this.dialog_close('#quickjs_js_dialog');
		},
        copy_to_clipboard: function(){
            this.clip_copy(this.downloaded_js);
            this.toast_show("Clipboard", "クリップボードにコピーしました。");
        },
    	upload_textarea: async function(){
            try{
                if( this.upload_mode == 'main')
                    await this.arduino.code_upload(this.input_text);
                else
                    await this.arduino.code_upload(this.input_text, this.module_name);
                this.toast_show("アップロード", "アップロードしました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
    	download_text: async function(){
            try{
                this.downloaded_js = await this.arduino.code_download();
                this.downloaded_fname = null;
                this.dialog_open("#quickjs_js_dialog");
            }catch(error){
                console.error(error);
                alert(error);
            }
    	},
    	save_textarea: function(){
	    	const url = URL.createObjectURL(new Blob([this.input_text], { type: "text/plain" }));
			const a = document.createElement("a");
			a.download = 'notitle.js';
			a.href = url;
			a.click();
			a.remove();
			URL.revokeObjectURL(url);
    	},
		catch_tabKey: function( e ){
			// Ctrl-sを無視
 			if (e.keyCode == 83 && e.ctrlKey){
				e.preventDefault();
				if( !confirm("アップロードしますか？") )
	 				return;
	 			this.save_textarea();
 			}
			// TABのみフック
			if( e.keyCode != 9 )
				return;
 
			e.preventDefault();

			var cursorPosition = e.target.selectionStart;
			var cursorLeft     = e.target.value.substr( 0, cursorPosition );
			var cursorRight    = e.target.value.substr( cursorPosition, e.target.value.length );

			e.target.value = cursorLeft+"\t"+cursorRight;
			e.target.selectionEnd = cursorPosition + 1;
		}, 
        download_module: async function(){
            try{
                this.downloaded_js = await this.arduino.code_download(this.target_module);
                this.downloaded_fname = this.target_module;
                this.dialog_open("#quickjs_js_dialog");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        get_module_list: async function(){
            try{
                var ret = await this.arduino.code_list();
                console.log(ret);
                this.module_list = ret;
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
        delete_module: async function(){
            try{
                await this.arduino.code_delete(this.target_module);
                alert("削除しました。");
            }catch(error){
                console.error(error);
                alert(error);
            }
        },
    },
    created: function(){
    },
    mounted: function(){
        proc_load();
    
        this.wires = [this.Wire, this.Wire1];
    }
};
vue_add_data(vue_options, { progress_title: '' }); // for progress-dialog
vue_add_global_components(components_bootstrap);
vue_add_global_components(components_utils);

/* add additional components */
  
window.vue = new Vue( vue_options );
