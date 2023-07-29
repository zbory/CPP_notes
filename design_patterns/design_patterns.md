# Tervezési minták

## Tag dispatch
Azonos szignatúrájú függvények szétválasztására. Azért hozunk létre osztályokat/struct-okat, hogy tag-ekként működjenek. Ugyanezt elérhetnénk enum-okkal, template-ekkel és futásidejű polimorfizmussal is.<br>

https://www.fluentcpp.com/2018/05/01/when-to-use-enums-and-when-to-use-tag-dispatching-in-cpp/#:~:text=Essentially%2C%20tags%20get%20dispatched%20at,dispatching%20relies%20on%20function%20overloading.<br>
[Example](hetfo/TagDispatch.cpp)
## Lérehozási minták
### Singleton
[Example](hetfo/Singleton.cpp)