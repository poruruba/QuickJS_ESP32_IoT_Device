import * as utils from "Utils";

export default
class Ambient{
	constructor(channelId, writeKey){
		this.channelId = channelId;
		this.writeKey = writeKey;
	}
	
	async send(data){
		var params = {
			writeKey: this.writeKey,
			data: (data instanceof Array) ? data : [data]
		};
		var ret = utils.httpPostJsonText('http://ambidata.io/api/v2/channels/' + this.channelId + '/dataarray', params);
		await this.sleep(500);
	}

	async sleep(msec){
		return new Promise(resolve => setTimeout(resolve, msec) );
	}
}
