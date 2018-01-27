phina.globalize();

var v = 5;
var size = 32;
var margin = 10;
var flag = false;//落下フラグ
var ASSETS = {
  image: {
    'hiyoko': 'hiyoko.png',
  },
  spritesheet: {
    'hiyoko_ss': {
      'frame': {
        'width': 32,
        'height': 32,
        'cols': 6,
        'rows': 3,
      },
      'animations': {
        'walk': {
          'frames': [1, 2, 3],
          'next': 'walk',
          'frequency': 10,
        }
      }
    }
  }
};

phina.define('MainScene', {
  superClass: 'DisplayScene',

  init: function(){
    this.superInit({
      width: 500,
      height: 500,
    });
    this.group = DisplayElement().addChildTo(this);
    this.hiyoko = Sprite('hiyoko').addChildTo(this);
    this.hiyoko.setPosition(250, 0);
    var anime = FrameAnimation('hiyoko_ss').attachTo(this.hiyoko);
    anime.gotoAndPlay('walk');
  },
  update: function(app){
    var key = app.keyboard;
    var x = this.hiyoko.x;
    var y = this.hiyoko.y;
    this.hiyoko.y += v;
    //落下あたり判定
    for(var i = 0; i < this.group.children.length; i++){
      if(this.hiyoko.hitTestElement(this.group.children[i])){
        //落下してぶつかるなら落下しない
        this.hiyoko.y -= v;
      }
    }

    if(key.getKey('a')){
      this.hiyoko.x -= v;
      this.hiyoko.scale.x = 1;
    }
    if(key.getKey('d')) {
      this.hiyoko.x += v;
      this.hiyoko.scale.x = -1;
    }
    //上下移動
    /*
    if(key.getKey('s')) {
      this.hiyoko.y += v;
    }
    if(key.getKey('w')) {
      this.hiyoko.y -= v;
    }
    */
    //操作当たり判定
    for(var i = 0; i < this.group.children.length; i++){
      if(this.hiyoko.hitTestElement(this.group.children[i])){
        //ぶつかっていた際、前の状態に戻す
        this.hiyoko.x -= v;
        //this.hiyoko.y -= v;
      }
    }
    //block生成
    if(app.frame % 60 == 0){
      for(var i = 1; i <= 15; i++){
        var rect = RectangleShape().addChildTo(this.group);
        rect.setPosition(i*size, 500);
        rect.width = size;
        rect.height = size;
      }
    }
    //2秒ごとにブロックを上にあげる
    if(app.frame % 60 == 0){
      for(var i = 0; i < this.group.children.length; i++){
        this.group.children[i].y -= size;
        //持ち上げた際にひよこも持ち上げる
        if(this.hiyoko.hitTestElement(this.group.children[i])){
          this.hiyoko.y -= size;
          console.log("hiyoko.y:"+ this.hiyoko.y + " block.y:"+ this.group.children[i].y);
        }
      }
    }
  }
});

phina.main(function(){
  var app = GameApp({
    startLabel: 'main',
    assets: ASSETS,
    fps: 60,
    fit: false,
    width: 500,
    height: 500,
  });
  app.run();
});

var rand = function(n){
  return Math.floor(Math.random() * n) + 1
}