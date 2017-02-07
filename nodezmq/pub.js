var zmq = require('zmq')
  , sock = zmq.socket('push');

sock.bindSync('tcp://*:8888');
console.log('Producer bound to port 8888');

setInterval(function(){
  console.log('sending work');
  sock.send('some work');
}, 500);