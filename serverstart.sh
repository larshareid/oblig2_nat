
#! /bin/sh
cd /var/www/

rm test.html
cat <<EOF >test.html
<!DOCTYPE html>
<html>
<body>

<h1>My First Heading</h1>
<p>My first paragraph.</p>

</body>
</html>
EOF

rm test.asis
cat <<EOF >test.asis
This is a 
test if the file
is  created
EOF

rm test.xml
cat <<EOF >test.xml
<breakfast_menu>
<food>
<name>Belgian Waffles</name>
<price>$5.95</price>
<description>Two of our famous Belgian Waffles with plenty of real maple syrup</description>
<calories>650</calories>
</food>
<food>
<name>Strawberry Belgian Waffles</name>
<price>$7.95</price>
<description>Light Belgian waffles covered with strawberries and whipped cream</description>
<calories>900</calories>
</food>
<food>
<name>Berry-Berry Belgian Waffles</name>
<price>$8.95</price>
<description>Light Belgian waffles covered with an assortment of fresh berries and whipped cream</description>
<calories>900</calories>
</food>
<food>
<name>French Toast</name>
<price>$4.50</price>
<description>Thick slices made from our homemade sourdough bread</description>
<calories>600</calories>
</food>
<food>
<name>Homestyle Breakfast</name>
<price>$6.95</price>
<description>Two eggs, bacon or sausage, toast, and our ever-popular hash browns</description>
<calories>950</calories>
</food>
</breakfast_menu>
EOF

rm test.json
cat <<EOF >test.json
{
    "glossary": {
        "title": "example glossary",
		"GlossDiv": {
            "title": "S",
			"GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
					"SortAs": "SGML",
					"GlossTerm": "Standard Generalized Markup Language",
					"Acronym": "SGML",
					"Abbrev": "ISO 8879:1986",
					"GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
						"GlossSeeAlso": ["GML", "XML"]
                    },
					"GlossSee": "markup"
                }
            }
        }
    }
}
EOF

rm test.txt
cat <<EOF >test.txt
This is a 
test if the file
is  created
EOF

rm test.css
cat <<EOF >test.css
/* Applies to the entire body of the HTML document (except where overridden by more specific
selectors). */
body {
  margin: 25px;
  background-color: rgb(240,240,240);
  font-family: arial, sans-serif;
  font-size: 14px;
}

/* Applies to all <h1>...</h1> elements. */
h1 {
  font-size: 35px;
  font-weight: normal;
  margin-top: 5px;
}

/* Applies to all elements with <... class="someclass"> specified. */
.someclass { color: red; }

/* Applies to the element with <... id="someid"> specified. */
#someid { color: green; }
EOF

cd
cd /var/log
rm debug.log
touch debug.log

cd
gcc /home/lars/testServer/forkv1.c -o fork
sudo ./fork 2>> /var/log/debug.log



echo "Server running"

cd
rm pross.txt
ps -eaf | grep ./fork | grep nobody | awk '{print $2}' | cat $2 > pross.txt
