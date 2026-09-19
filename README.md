# FunBox Workshop

In diesem Workshop entwickeln wir gemeinsam ein Spiel für die **FunBox**.

Als Ausgangspunkt verwenden wir **Snake** und erweitern das Spiel Schritt für Schritt um neue Spielmechaniken.

## Vorbereitung

Für den Workshop benötigst du:

* Arduino IDE 2
* einen eingerichteten ESP32-C3
* die FunBox Library
* alle Dependencies der FunBox Library

## FunBox Library installieren

Die benötigte Library findest du hier:

https://github.com/anfark/funbox-library

Folge dort der Anleitung im README, um:

1. die ESP32-Tools einzurichten,
2. die benötigten Arduino-Libraries zu installieren und
3. die FunBox Library zu installieren.

## Workshop-Projekt öffnen

Lade dieses Repository herunter und öffne anschließend:

```text
funbox-workshop.ino
```

mit der Arduino IDE.

Das Projekt verwendet die FunBox Library:

```cpp
#include <FunBox.h>
```

Die Spiele werden beim Start registriert:

```cpp
FunBox box;

void setup() {
  box.addGame<SnakeGame>();

  box.setup();
}

void loop() {
  box.update();
}
```

## Board auswählen

Verbinde die FunBox per USB mit deinem Computer.

Wähle anschließend in der Arduino IDE:

```text
Tools → Board → ESP32C3 Dev Module
Tools → USB CDC On Boot → Enabled
Tools → Port → <deine FunBox>
```

Danach kannst du das Projekt mit **Upload** auf die FunBox übertragen.

## Workshop

Wir starten mit einer funktionierenden Version von Snake und erweitern sie gemeinsam.

Dabei beschäftigen wir uns mit:

* dem **State** des Spiels,
* der **Update-Logik**,
* **Events**,
* dem **Rendering** auf Matrix, Display und Audio.

Im Workshop erweitern wir Snake unter anderem um mehrere Früchte und zeitlich begrenzte Früchte.

Viel Spaß! 🐍
