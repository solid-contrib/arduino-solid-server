# arduino-solid-server
Crude server returning data in turtle from analog, digital, and temperature sensors of an arduino

## Example output

```
@prefix : <http://www.w3.org/ns/pim/arduino#>.

@prefix this: <#>.


this:thisDevice :state [
:a0 31;
:a1 159;
:a2 202;
:a3 251;
:a4 258;
:a5 285;

:d0 1;
:d1 1;
:d2 0;
:d3 1;
:d4 0;
:d5 0;
:d6 0;
:d7 0;
:d8 0;
:d9 0;
:d10 1;
:d11 1;
:t0 20.76;
:t1 21.40;
:t2 15.31
].
```

## Security considerations

This server does not use any authentiucation!  It must only be used in a closed trusted IoT net, protected by firewalls or proxies. This is just a quick demo, not a product, and no responsability is taken for its security in any way.

Beware of setting up systems which are ROM-based and cannot be upgraded over the Internet.  If a bug should manifest itself, they wil all become vulnerable at once, with no way to upgrade. 

