# Default Object Options
Edit some object values on place. (now with overcomplicated json support :3)

If you dont wanna mess with jsons, you can use the dont fade, dont enter, no glow toggles in mod settings. (thats what most ppl will use this for anyway)

## if you do wanna mess with jsons :3

no grammar, im lazy, uwu

im assuming you know the very basics of json, please dont be using this if you dont understand how to even format one :sob:

json goes in geodes config folder, in a folder named **"nwo5.default_object_options"** (click the folder icon in the geode menu and if you dont see a folder called **"config"** go back 1 folder in the hierarchy then u should see it)

the json is formatted like so

```
// entries.json

{
    "OBJECT ID": {
        "groups": [1, 2, 3],
        "toggles": {
            "TOGGLE": true,
            "TOGGLE 2": true
        },
        "scale": [SCALE/SCALEX, SCALEY]
    }
}
```

you can include, or not include, anything (other than the object id ofc), so u dont need a groups toggles and scale for every configuration

object id is pretty self explanatory, one thing is if u set the object id to 0 *all* objects will get configured on place with whatever you specify

groups is just an array

toggles has 6 things you can configure (all false by default)

- "dont-fade"
- "dont-enter"
- "no-effects"
- "no-touch"
- "no-particles"
- "no-glow"

you can set them to false technically but it wont do anything soooo

for scale you can either input ```[0.5]``` if you want the object to be scaled to 0.5 or ```[0.5, 1]``` if you wanna specify x and y scale