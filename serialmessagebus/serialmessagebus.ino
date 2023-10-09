/*keep this:buss:begin:*/
String _cmd_args_1 = "";
String _cmd_args_2 = "";
String _cmd_args_3 = "";
String _all_data_from_outside = "";

int parse_command() {
  int idx = _all_data_from_outside.indexOf("\r\n\r\n");
  if (idx < 0) return 0;
  idx = idx + 4;
  String cmdanddata = _all_data_from_outside.substring(0, idx);
  _all_data_from_outside = _all_data_from_outside.substring(idx);

  idx = cmdanddata.indexOf("\r\n");
  if (idx < 0) return 1;
  _cmd_args_1 = cmdanddata.substring(0, idx);
  cmdanddata = cmdanddata.substring(idx + 2);

  idx = cmdanddata.indexOf("\r\n");
  if (idx < 0) return 1;
  _cmd_args_2 = cmdanddata.substring(0, idx);
  cmdanddata = cmdanddata.substring(idx + 2);

  idx = cmdanddata.indexOf("\r\n");
  if (idx < 0) return 1;
  _cmd_args_3 = cmdanddata.substring(0, idx);
  cmdanddata = cmdanddata.substring(idx + 2);

  return 1;
}

void handle_command(String cmd, String time, String data) {
  //todo: your logic
  //test send back: cmd==testcmd
  send_command(cmd, millis(), data + " handled from arduino");
}

void send_command(String cmd, long time, String data) {
  String x1 = cmd + String("\r\n") + String(time) + String("\r\n") + data + String("\r\n\r\n");
  Serial.write(x1.c_str());
}

void init_buss() {
  String cmdraw = Serial.readString();
  if (cmdraw != "") {
    _all_data_from_outside = _all_data_from_outside + cmdraw;
    while (parse_command() == 1) {
      handle_command(_cmd_args_1, _cmd_args_2, _cmd_args_3);
    }
  }
}
/*keep this:buss:end:*/

//todoo: add you code here, can switch case cmd in handle_command

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  Serial.setTimeout(1);
}
void loop() {
  init_buss();

  delay(1);
}
