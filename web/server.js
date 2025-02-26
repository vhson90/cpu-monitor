const express = require("express");
const mysql = require("mysql2");
const cors = require("cors");
const http = require("http");
const socketIo = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
    cors: { origin: "*" }
});

app.use(cors());

const db = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "123456",
    database: "Sensors"
});

db.connect(err => {
    if (err) {
        console.error("MySQL connection failed:", err);
        return;
    }
    console.log("Connected to MySQL");
});

const getLatestData = () => {
    const tempQuery = "SELECT stt, DATE_FORMAT(date_time, '%H:%i:%s') AS date_time, CPU, SSD FROM temp_sensor ORDER BY date_time DESC LIMIT 10";
    const batteryQuery = "SELECT stt, DATE_FORMAT(date_time, '%H:%i:%s') AS date_time, charge, voltage FROM battery ORDER BY date_time DESC LIMIT 10";
    db.query(tempQuery, (err, tempResults) => {
        if (err) {
          console.error("Errỏ on fetching temp_sensor:", err.message);
          return;
        }
    
        db.query(batteryQuery, (err, batteryResults) => {
          if (err) {
            console.error("Error on fetching battery:", err.message);
            return;
          }
    
          // Gửi dữ liệu mới nhất đến client
          io.emit("sensorData", {
            temp_sensor: tempResults.reverse(),
            battery: batteryResults.reverse()
          });
        });
      });
};

setInterval(getLatestData, 1000);

io.on("connection", (socket) => {
    console.log("Client connected:", socket.id);
    
    // Gửi dữ liệu ngay khi client kết nối
    getLatestData();
  
    socket.on("disconnect", () => {
      console.log("Client disconnected:", socket.id);
    });
  });

server.listen(3000, () => {
    console.log("Server running on http://localhost:3000");
});
