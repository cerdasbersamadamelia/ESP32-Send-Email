#include "ESP32_MailClient.h"
#include "SD.h"
#include "SPIFFS.h"

const char* ssid = "nama wifi";
const char* password = "password wifi";

#define emailSenderAccount    "pengirim@gmail.com"
#define emailSenderPassword   "password pengirim"
#define emailRecipient        "penerima@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test Email with Attachments"

SMTPData smtpData;

void sendCallback(SendStatus info);

void setup(){
  Serial.begin(115200);
  Serial.println();
  
  if(!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  Serial.print("Connecting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();
  Serial.println("Preparing to send email");
  Serial.println();
  
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("ESP32", emailSenderAccount);
  smtpData.setPriority("High");
  smtpData.setSubject(emailSubject);
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hi There</h1><p>Sent from ESP32 board</p></div>", true);
  smtpData.addRecipient(emailRecipient);
  smtpData.addAttachFile("/hi.jpg", "image/jpg");
  smtpData.addAttachFile("/file_text.txt");
  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
  smtpData.setSendCallback(sendCallback);

  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  smtpData.empty();
}

void loop() {
}

void sendCallback(SendStatus msg) {
  Serial.println(msg.info());

  if (msg.success()) {
    Serial.println("----------------");
  }
}
