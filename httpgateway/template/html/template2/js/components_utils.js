const components_utils = {
  comp_file: {
    props: ['id', 'callback', 'accept', 'multiple'],
    template: `
    <div>
      <input type="file" class="form-control" v-bind:id="id" v-bind:accept="accept" v-on:change="file_open" v-on:click="file_click" v-bind:multiple="multiple">
    </div>
    `,
    methods: {
      file_drag: function (e) {
        e.stopPropagation();
        e.preventDefault();
      },
      file_click: function (e) {
        e.target.value = '';
        if(this.callback)
          this.callback([]);
      },
      file_open: function (e) {
        this.file_open_files(e.target.files);
      },
      file_drop: function (e) {
        e.stopPropagation();
        e.preventDefault();

        document.querySelector('#' + this.id).files = e.dataTransfer.files;
        this.file_open_files(e.dataTransfer.files);
      },
      file_open_files: function (files) {
        if( this.callback )
          this.callback(files);
      },
      file_reset: function(){
        document.querySelector('#' + this.id).value = "";
      }
    }
  },
}