# MSFHAC_LixieClock

Die Lixie-Uhr des Makerspace der FH Aachen

![4_digit_complete_alt](./documentation/images/4_digit_complete_alt.jpg)


![4_digit_on_2](./documentation/images/4_digit_on_2.jpg)




# WERKZEUG

* Lötkolben + Lötzinn
* Philips-Kopf Schraubendreher
* Kleine Kabelbilder
* Heißkleber
* Glasreiniger
* Mikro-USB Kabel
* [OPTINAL] Holzkleber

# TEILELISTE

## BIlD ZEIGT ALTE REVISION
![4_digit_parts](./documentation/images/4_digit_parts.jpg)


## MECHANICAL

* LASERCUT TEILE
* *  4x `./src/svg/4x_numbers_2mm_set_300_200` - acrylglas gs 2mm, engrave (blue lines), cut (red lines)
* *  1x `./src/svg/4_digit_combined_set_1` - playwood 4mm, cut (red lines), engrave (blue lines)
* *  1x `./src/svg/4_digit_combined_set_2` - playwood 4mm, cut (red lines), engrave (blue lines)
* *  1x `./src/svg/4_digit_combined_set_3` - playwood 4mm, cut (red lines), engrave (blue lines)


* 16x `M4x30 Philips Head Type PH2 - DIN7985`
* 16x `M4 Nut - DIN934`

## ELECTRICAL

* 4x LED PCB, SEE `./src/pcb`, LED SEITE BESTÜCKT
* 1x ESP8266 D1 Mini
* 8* `1*3 2.54mm  90° MALE HEADERS`
* 4x 3x`Jumperwires FEMALE<=>FEMALE`






## BUILD INSTRUCTIONS


### 0. PROGRAMMIERUNG BASIS-PROGRAMM





### 1. LED MODULE VORBEREITEN


![single_digit_new](./documentation/images/single_digit_new.jpg)

Jedes LED Modul besteht aus drei Schichten:

* 4x 10x Arcyglglas Ziffern
* 4x 1x Sperrholz Ziffern-Halter
* 4x 1x Sperrholz Lightguide


Zuerst wird die Schtzfolien von den einzelnen Ziffern entfernt und anschließend mit Glasreiniger entfettet.
Ab jetzt dürfen die Ziffern nur noch an den Seiten angefasst werden, denn nach der Montage in die Halterung kommt man nur noch sehr schwer an die Flächen.

Jetzt werden sie einzelnen Ziffern in den `Ziffern-Halter` eingesetzt. Dabei müssen diese mit der gravierten Seite alle  in die gleiche Richtung zeigen.
Auch ist die Reihenfolge wichtig!
Von hinten nach vorne, sind die Ziffern in dieser Reihenfolge einzusetzten:

`0` - `9`- `8` - `7` - `6` - `5` - `4` - `3` - `2` - `1`  


![single_digit_digits_only](./documentation/images/single_digit_digits_only.jpg)

Anschließend werden die Ziffern auf den `Lighguide` gesetzt. Dabei muss die gravierte Seite der Ziffern in  Richtung der `FRONT` Markierung zeigen und die Markierungen auf dem `Lightguide` müssen mit den Ziffern übereinstimmen.

Dieser Prozess, wird für alle vier Ziffern-Blöcke wiederholt.



#### NOTE

![pcb_solder](./documentation/images/pcb_solder.jpg)

Sollte bei den vier `LED-PCB`s die zwei Stiftleisten nicht bestückt sein, muss dies zuerst geschehen.
Dazu werden diese in die beiden vorgesehenden 1x3 Pin Bohrungen gesteckt. Diese sind auf der Platine mit `H1` und `H2` beschriftet.
Die lange Seite der Stiftleisten zeigt dabei von der LED Seite weg.

### 2. Zusammenbau Oberseite

Nachdem alle vier Module soweit vorbereitet wurden, können diese nun mit der oberen Bodenplatte zusammengeschraubt werden.
Dabei wird auch die LED PCB und der `PCB Spacer` zusätzlich benötigt.

* 4x PCB Spacer
* 4x LED_PCB mit Bestückter LED Seite
* 4x Top-Cover


Für den Zusammenbau, wird das `TOP_COVER` mit der Beschriftung nach oben gelegt und die `LED-PCB`s mit den Stiftleisten ind die Aussüarungen gelegt.

**WICHTIG ** Die `FRONT` Markeierung auf dem `TOP_COVER` und der Pfeil zeigt dabei zur Tischkante!

Diese werden dadurch nicht flach auf dem Tisch liegen, es Empfiehlt sich das `TOP_COVER` dafür auf eine Tasse zu stellen.

![led_pcb_placement](./documentation/images/led_pcb_placement.jpg)


Anschließend wird auf jede `LED_PCB` der Sperrholz `PCB_SPACER` gelegt, hier ist die Ausrichtung nicht relevant.
Als letztes wird das im ersten Schritt zusammengebaute Ziffern-Modul auf den `PCB_Spacer` gesetzt.
Dabei ist die Ziffer `1` die vorderste Ziffer (richtung der `FRONT` MARKIERUNG).
Nach dem Platzieren der vier Ziffern-Module, sollten diise jeweils mit einer der `M40x30` Schrauben und `M4` Mutter gesichert werden.
Hier reicht es diese nicht ganz fest anzuziehen. Dieser Schritt erleichter die Montage im folgendnen Schritt.

![led_module_and_top_cover](./documentation/images/led_module_and_top_cover.jpg)

### 3. Montage Bodenplatte

![spacers](./documentation/images/spacers.jpg)

Für diesen Schritt, wird alles umgedreht, sodass die LED Module mit der runden Seite auf der Tischplatte stehen.
Die `FRONT` Markierung sollte jetzt in die andere Richtung Zeigen und somit das LED-Modul welches die Zehner-Stunden darstellen auf der linken  Seite befinden.
Hier werden die folgenden Sperrholzteile benötigt:

* 1x `SPACER_TOP`
* 1x `SPACER_BOTTOM`

Diese dienen als Abstandshalter zwischen des `TOP_COVERS` und der Bodenplatte `BOTTOM_COVER`.
Am `SPACER_BOTTOM` befindet sich eine Aussparung für den `ESP8266 D1 MINI`.

#### OPTINAL 
Da diese beiden Teile leicht zerbrechlich sind bietet es sich an, diese mit etwas Holzleim zu verkleben.

![spacers_glue](./documentation/images/spacers_glue.jpg)

Dabei wird der  `SPACER_TOP` auf den `SPACER_BOTTOM` geklebt und zu einfachen Arretierung dieser mit zwei Schrauben für 5 Minuten verschraubt.
Nach dieser Zeit können diese wieder gelößt werden.

![spacer_final](./documentation/images/spacer_final.jpg)


#### ENDE OPTIONAL


Die Schrauben (aus Schritt 2), welche die Module gehalten haben können jetzt wieder gelößt werden.
Die beiden Spacer (geklebt oder nicht), werden anschlißend auf das `TOP_COVER` gesetzt, sodass sich die Aussparung unten links in der Ecke befindet und sich somit auf der Rückseite der Uhr befindet.
Anschlißend können die Module wieder mit je einer Schraube fixiert werden. Somit sollten auch die beiden Spacer fixiert sein.


### 4. Montage Elektronik

Für die Verkabelung und Montage der Elektronik, wird alles umgedreht, sodass die LED Module mit der runden Seite auf der Tischplatte stehen.
Die `FRONT` Markierung sollte jetzt in die andere Richtung Zeigen und somit das LED-Modul welches die Zehner-Stunden darstellen auf der linken  Seite befinden.
Für diesen Schritt werden die `Jumper-Wires`, sowie der `ESP8266 D1 MINI` benötigt.

Jetzt werden mit den 4x 3x `Jumper-Wires` die einzelnen Module miteinander Verbunden. Auf den `LED-PCB`s sollten die Stiftleisten durch das `TOP_COVER` zugänglich sein. Bei diesen sind die einzelnen Pins beschriftet. Für `H1 => 5V D0 GND` und für `H2 => 5V DIN GND`. Das Ziel ist es die einzelnen Platinen zu einer Kette zu verbinden. 
Angefangen von der Zehner-Stunden, wird die `H2`-Stiftleiste über drei `Jumper-Wires` mit der `H1`-Stiftleiste des Stunden-Einer-Moduls verbunden.
Dabei wird `5V->5V`, `GND->GND` und `DI->D0` verbunden.
Dieser Prozess wird anschließend noch zweimal wiederholt:

* Stunden-Zehner `H2` => Stunden-Einer `H1`
* Stunden-Einer `H2` => Minuten-Zehner `H1`
* Minuten-Zehner `H2` => Minuten-Einer `H1`

Nach diesem Schritt sind die Stiftleisten  `Stunden-Zehner H1` und `Minuten-Einer H2` nicht verbunden.
Im letzten Schritt wird der `ESP8266 D1 Mini` mit der `Stunden-Zehner H1` verbunden.
Dabei werden werden die folgenden Pins am `ESP8266 D1 Mini` verunden:

* `(ESP8266 D1 MINI) 5V` => `(Zehner Stunden H1) 5V`
* `(ESP8266 D1 MINI) D8` => `(Zehner Stunden H1) DIN`
* `(ESP8266 D1 MINI) GND` => `(Zehner Stunden H1) GND`

Anschließend kann vor dem finalen Zusammenbau die Funktion getestet werden.
Mit einem USB-Kabel wird der `ESP8266 D1 Mini` verbunden. Jede Ziffer sollte einmal aufleuchten.
Falls dies nicht der Fall ist, ist die Verkabelung zu Prüfen.

Um den Zusammenbau zu vereinfachen, werden die einzelnen Stifleisten mit dem `Jumperwires` leicht zueinender (ca. 45°) gebogen und mit Heisskleber fixiert.
Auch die Kabel ziwschen den Elementen werden mit Kabelbindern und/oder Heisskleber an der `TOP_PLATE` fixiert.

![electric_complete](./documentation/images/electric_complete.jpg)

### 5. Endmontage

Im letzten Schritt wirde der `ESP8266 D1 Mini` in die Aussparung des Spacers mit Heisskleber geklebt, sodass der USB-Anschluss nach aussen zeigt.
Geklebt wird auf der glatten Seite des `ESP8266 D1 Mini` (auf der gegenüberliegenden Seite des USB Anschluss).

![esp8266_usb](./documentation/images/esp8266_usb.jpg)

Final kann die Bodenplatte `BOTTOM_COVER` Montiert werden. Dazu werden die zuvor zur Montage genutzten Schrauben wieder gelösst und das `BOTTOM_COVER` auf die Spacer gelegt. Alle Schrauben werden dann handfest angezogen.


![bottom_screw](./documentation/images/bottom_screw.jpg)
