# Tervezési minták

## Tag dispatch
The “tag” in tag dispatching refers to a type that has no behaviour and no data

Azonos szignatúrájú függvények szétválasztására. Azért hozunk létre osztályokat/struct-okat, hogy tag-ekként működjenek. <br>(Ugyanezt elérhetnénk enum-okkal, template-ekkel és futásidejű polimorfizmussal is.)<br>

Enum VS Tag dispatch: fő különbség
- logika egy függvényben vagy több függvénybe szétválasztva
- a feloldás ideje futási időben vagy fordítási időben



[Example](hetfo/TagDispatch.cpp)

* https://www.fluentcpp.com/2018/04/27/tag-dispatching/
* https://www.fluentcpp.com/2018/05/01/when-to-use-enums-and-when-to-use-tag-dispatching-in-cpp/#:~:text=Essentially%2C%20tags%20get%20dispatched%20at,dispatching%20relies%20on%20function%20overloading.<br>
## Lérehozási minták
### Singleton
[Example](hetfo/Singleton.cpp)