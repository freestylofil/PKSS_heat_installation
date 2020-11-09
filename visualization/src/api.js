import openSocket from 'socket.io-client';
const socket = openSocket('http://localhost:5000/test');


function subscribeToTimer(interval, cb) {
    socket.on('test_event', timestamp => cb(null, timestamp));
    socket.emit('subscribeToTimer', 1000);
} 
export { subscribeToTimer }