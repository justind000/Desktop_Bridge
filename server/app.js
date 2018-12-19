var app = new Vue({
  el: "#app",
  data: {
    socket: 0,
    characteristic: 0,
    slot_1_value: "-",
    slot_1_unit: "-",
    slot_2_value: "-",
    slot_2_unit: "-",
    slot_3_value: "-",
    slot_3_unit: "-",
    temp: "-",
    temp_unit: "-",
    name: "disconnected",
    pH_connected: false,
    EC_connected: false,
    ORP_connected: false,
    pH_Low_Reading: "-",
    pH_Low_Reference: "-",
    pH_High_Reading: "-",
    pH_High_Reference: "-",
    pH_DP_Enabled: false,
    pH_TC_Enabled: false,
    pH_offset: "-",
    connected: false,
    show_config: false,
    EC_calibration: "-",
    SW_calibration: "-",
    EC_TC_Enabled: false,
    EC_temp_const: "-"
  },
  watch: {
    pH_DP_Enabled: function(val) {
      if (app.pH_DP_Enabled) {
        app.socket.send("dp 1");
      } else {
        app.socket.send("dp 0");
      }
    },
    pH_TC_Enabled: function(val) {
      if (app.pH_TC_Enabled) {
        app.socket.send("ph_tc 1");
      } else {
        app.socket.send("ph_tc 0");
      }
    },
    EC_TC_Enabled: function(val) {
      if (app.EC_TC_Enabled) {
        app.socket.send("ec_tc 1");
      } else {
        app.socket.send("ec_tc 0");
      }
    }
  },
  methods: {
    connect: function(event) {
      if (this.socket.readyState === this.socket.CLOSED) {
        let websocketurl = "ws://localhost:5678/ws";
        this.socket = new WebSocket(websocketurl);

        this.socket.onerror = function() {
          app.name = "Connection Error: is the WebSockets server running?";
        };

        this.socket.onopen = function() {
          app.connected = true;
          //app.socket.send("mv");
          app.socket.send("ph_connected");
          app.socket.send("ec_connected");
          app.socket.send("orp_connected");
          app.socket.send("t");
          app.socket.send("ph");
          app.socket.send("ec");
          app.socket.send("orp");
          setInterval(app.measure, 5000);
        };

        this.socket.onclose = function() {
          app.disconnect();
        };

        this.socket.onmessage = function(message) {
          let obj = JSON.parse(message.data);
          console.log(obj);
          if (obj.hasOwnProperty("pH_connected")) {
            app.pH_connected = obj.pH_connected;
          }
          if (obj.hasOwnProperty("EC_connected")) {
            app.EC_connected = obj.EC_connected;
          }
          if (obj.hasOwnProperty("ORP_connected")) {
            app.ORP_connected = obj.ORP_connected;
          }
          if (obj.hasOwnProperty("slot_1_value")) {
            app.slot_1_value = obj.slot_1_value;
          }
          if (obj.hasOwnProperty("slot_1_unit")) {
            app.slot_1_unit = obj.slot_1_unit;
          }
          if (obj.hasOwnProperty("slot_2_value")) {
            app.slot_2_value = obj.slot_2_value;
          }
          if (obj.hasOwnProperty("slot_2_unit")) {
            app.slot_2_unit = obj.slot_2_unit;
          }
          if (obj.hasOwnProperty("slot_3_value")) {
            app.slot_3_value = obj.slot_3_value;
          }
          if (obj.hasOwnProperty("slot_3_unit")) {
            app.slot_3_unit = obj.slot_3_unit;
          }
          if (obj.hasOwnProperty("slot_1_value")) {
            app.slot_1_value = obj.slot_1_value;
          }
          if (obj.hasOwnProperty("slot_1_value")) {
            app.slot_1_unit = obj.slot_1_unit;
          }
          if (obj.hasOwnProperty("temp")) {
            app.temp = obj.temp;
          }
          if (obj.hasOwnProperty("temp_unit")) {
            app.temp_unit = obj.temp_unit;
          }
          if (obj.hasOwnProperty("pH_Low_Reading")) {
            app.pH_Low_Reading = obj.pH_Low_Reading;
          }
          if (obj.hasOwnProperty("pH_Low_Reference")) {
            app.pH_Low_Reference = obj.pH_Low_Reference;
          }
          if (obj.hasOwnProperty("pH_High_Reading")) {
            app.pH_High_Reading = obj.pH_High_Reading;
          }
          if (obj.hasOwnProperty("pH_High_Reference")) {
            app.pH_High_Reference = obj.pH_High_Reference;
          }
          if (obj.hasOwnProperty("pH_DP_Enabled")) {
            app.pH_DP_Enabled = obj.pH_DP_Enabled;
          }
          if (obj.hasOwnProperty("pH_TC_Enabled")) {
            app.pH_TC_Enabled = obj.pH_TC_Enabled;
          }
          if (obj.hasOwnProperty("pH_offset")) {
            app.pH_offset = obj.pH_offset;
          }
          if (obj.hasOwnProperty("EC_calibration")) {
            app.EC_calibration = obj.EC_calibration;
          }
          if (obj.hasOwnProperty("SW_calibration")) {
            app.SW_calibration = obj.SW_calibration;
          }
          if (obj.hasOwnProperty("EC_temp_const")) {
            app.EC_temp_const = obj.EC_temp_const;
          }
          if (obj.hasOwnProperty("EC_TC_Enabled")) {
            app.EC_TC_Enabled = obj.EC_TC_Enabled;
          }
          app.name = obj.name;
        };
      } else {
        {
          this.disconnect();
        }
      }
    },
    disconnect: function() {
      this.socket.close();
      this.connected = false;
      this.name = "Disconnected";
      this.slot_1_value = "-";
      this.slot_1_unit = "-";
      this.slot_2_value = "-";
      this.slot_2_unit = "-";
      this.slot_3_value = "-";
      this.slot_3_unit = "-";
      this.temp = "-";
      this.temp_unit = "-";
    },
    measure: function() {
      app.socket.send("t");
      app.socket.send("ph");
      app.socket.send("ec");
      app.socket.send("orp");
    },
    ph_config_modal: function() {
      app.socket.send("low");
      app.socket.send("high");
      app.socket.send("dp");
      app.socket.send("ph_tc");
      app.socket.send("ph_cal");

      var modal = document.querySelector(".ph_config_modal");
      var html = document.querySelector("html");
      modal.classList.add("is-active");
      html.classList.add("is-clipped");

      modal
        .querySelector(".ph_modal_close")
        .addEventListener("click", function(e) {
          e.preventDefault();
          modal.classList.remove("is-active");
          html.classList.remove("is-clipped");
        });
    },
    ph_calibrate: function() {
      var ph = prompt("Calibration solution pH?", "7.0");
      if (ph != null) {
        app.socket.send("ph_cal " + ph);
      }
    },
    ph_low: function() {
      var ph = prompt("Low calibration solution pH?", "4.0");
      if (ph != null) {
        app.socket.send("low " + ph);
      }
    },
    ph_high: function() {
      var ph = prompt("High calibration solution pH?", "10.0");
      if (ph != null) {
        app.socket.send("high " + ph);
      }
    },
    ph_reset: function() {
      app.socket.send("ph_reset");
      app.socket.send("low");
      app.socket.send("high");
      app.socket.send("dp");
      app.socket.send("ph_tc");
      app.socket.send("ph_cal");
    },
    ec_config_modal: function() {
      app.socket.send("cal-ec");
      app.socket.send("cal-sw");
      app.socket.send("ec_tc");
      app.socket.send("ec_temp_const");

      var modal = document.querySelector(".ec_config_modal");
      var html = document.querySelector("html");
      modal.classList.add("is-active");
      html.classList.add("is-clipped");

      modal
        .querySelector(".ec_modal_close")
        .addEventListener("click", function(e) {
          e.preventDefault();
          modal.classList.remove("is-active");
          html.classList.remove("is-clipped");
        });
    },
    calibrate_ec: function() {
      var ec = prompt("Calibration solution in mS?", "2.0");
      if (ec != null) {
        app.socket.send("cal-ec " + ec);
      }
    },
    calibrate_sw: function() {
      var ec = prompt("Calibration solution in mS?", "53.0");
      if (ec != null) {
        app.socket.send("cal-sw " + ec);
      }
    },
    temp_const: function() {
      var c = prompt("Temperature constant to adjust to, in C?", "25.0");
      if (c != null) {
        app.socket.send("ec_temp_const " + c);
      }
    },
    ec_reset: function() {
      app.socket.send("ec_reset");
      app.socket.send("cal-ec");
      app.socket.send("cal-sw");
      app.socket.send("ec_tc");
      app.socket.send("ec_temp_const");
    }
  }
});
