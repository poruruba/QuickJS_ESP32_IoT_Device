class TextResponse{
    constructor(content_type, context){
        this.statusCode = 200;
        this.headers = {'Access-Control-Allow-Origin' : '*', 'Cache-Control' : 'no-cache', 'Content-Type': content_type};
        if( context )
            this.set_body(context);
        else
            this.body = "";
    }

    set_error(error){
        this.body = JSON.stringify({"err": error});
        return this;
    }

    set_body(content){
        this.body = content;        
        return this;
    }
    
    get_body(){
        return this.body;
    }
}

module.exports = TextResponse;
