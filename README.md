# JUST BEFORE

#### Video demo: <https://youtu.be/P3UCjQ5IDXU>
#### Description:

# JUST BEFORE
Just Before is a web-based application that allows the user to keep track of expiring date so that less and less food goes wasted.

## Idea and what I want to accomplish
For this project, the main objective was to create something useful that people could avail of every day. Monitoring my family’s lifestyle, one of the principal issues to face is food waste. Unfortunately, it is common to just throw away food because it has gone bad, without people even remembering they had bought it. After doing some research, my suspects were proven to be true: food waste is widespread in every part of the world (but especially in richer and developing countries) and has a huge impact on the environment and the poors. Thus, it’s highly important to try to reduce it, even if only to a small extent, and Just Before is created to help just with that. There are indeed several benefits of reducing food waste, including
*cutting down methane, carbon emissions and pollution in general
*saving money from buying less food
*helping communities, especially in low-income countries, that, on the other hand, are in need
So the goal for the user is clear: “to be able to eat everything **JUST BEFORE** it goes bad”

## Features I wanted to implement
With my idea in mind, I tried put myself in the users’ shoes, to identify all the features I would have wanted the web-application to have, thus making it both functional and user-friendly.
Specifically, they were:
•	allowing the user to register and then log in and out
•	allowing the user to make changes to the account
•	having visually available the list of products the user has already inserted, ordered by expiring date, so that it’s possible to organize meals effectively without nothing going bad
•	adding new products
•	search for a product
•	modify informations about products already inserted (ex. the quantity)
•	delete products once eaten or unfortunately spoiled

Let’s go through how I realized it step by step, explaining how the database is structured and analyzing each route and the corresponding html file.

### MYSQL DATABASE
The database essentially consists of two tables, called “users” and “products”.
The first one is used to store the users’ information, including their username, email and password, which was previously hashed for safety reasons, and the id necessary to identify each user.
The second is instead used to store products inserted by users, along with additional information including the expiring date, the quantity, and the unit of measurement.
To link correctly the two tables (so that, for instance, it is possible to know that products a specific user has inserted) the user-id is utilized as foreign key.

### INITIAL LAYOUT
It is a sort of blueprint for the other html files, which indeed start with some ninja syntax saying they extend layout.html.
In the head there are the meta tags, come links copied from Favicon to add the Favicon I created for my web application, the link to the css file and the title, containing the ninja block title to add additional text depending on what page the user is visiting.
When it comes to the body, the upper part is the bootstrap implementation of a navbar, which is present in every page of the web-app. However, after the logo “Just Before”, it has a different format depending on whether the user has logged in or not. Specifically, using the ninja syntax, if the field user_id of the flask session object is not None (meaning the user has logged in), the fields “Add”, “Search”, “Delete”, “Change” are shown on the left, while “Account” and “Log Out” on the right. On the other hand, if the user has not logged in yet, “Register” and “Log In” are displayed on the right.
The first element of the main part, shared by all pages, is instead a div with multiple colourful spans inside, which I used a a sort of background by putting its z-index in the css file equal to a negative value. Then there’s the main block jinja element to make the rest of the main different depending on the page visited by the user.
Hereafter, there is a part that is displayed, thanks to the if block provided by the ninja syntax, only if the user has not registered/logged in yet, since it’s aimed precisely at catching the user attention and convincing him of the importance of start using Just Before to reduce food waste. In fact, there are some statistics regarding how serious, problematic yet unfortunately common this phenomenon is. The data are organised by cards (from the bootstrap documentation), consisting of an image and a descriptive text, with some bolder and bigger titles in between.
Eventually in the footer, being the author, I just put my name.

### REGISTER
In the html file, there’s a form which allows the user to register by inserting his/her username, email and password. A confirmation for the password is also required.
At the end, there’s the “Register” button the user needs to click to submit the form.
In the #register route the first if … else permits to distinguish two different blocks of code, depending on the request method. If it is GET, it means the user is visiting the page by clicking on “register” in the navbar, so the user should simply be rendered the register template. On the other end, the POST method is used when the user is submitting the data to register by click the homonymous button. Therefore, the input, previously saved in appropriate variables using request.form.get, must be checked first, returning an apology if:
*a field was omitted
*the password and its confirmation do not match.
Moreover, since the username and the email need to be unique, db.execute is used twice to get the list of usernames and emails respectively already in the database. Then each item of the two lists is compared to the corresponding value inserted by the user and, if they are equal, another apology is returned.
Otherwise, the password is hashed using the generate_password_hash function from Werkzeug documentation, and the data is inserted in the user table using db.execute.

### LOGIN
In the html, a form is used to make the user insert his/her username and password.
After clicking the login button, in the corresponding #login route, request.form.get is used to get the input and check whether it was left blank, returning an apology if so.
Then, db.execute is invoked to control if the username provided is in the database and if the password is correct, and if either of the two do not check out, another apology is returned.
Otherwise, the user’s id is saved in the corresponding field of the session object, before redirecting to the main page.

### LOGOUT
It simply clears the flask session and redirects the user to the main layout page.

### ACCOUNT
Thanks to account, the user is given the chance to modify the data he/she has inserted while registering.
In the upper part of the page, below the navbar, there’s a grey container with the current user data, that is the username, the email and the password displayed as a sequence of asterisks.
Following, the form enables the user to change one or more fields by filling them out and clicking the “Change” button.
As usual, In the #account route, which is accessible only if users have previously logged in, the first if … else permits to distinguish two different blocks of code, depending on the request method. If it is GET, it means the user is visiting the page by clicking on “account” in the navbar so the page just needs to show the current account data and the form. Therefore, db.execute is used to select the information from the user table based on the session user id, and save them in a variable called userInfo, which is a list of dictionaries with actually just one element inside, which, in turn, is selected using the bracket notation and stored in a variable called displayInfo. However, the field containing the password is changed to a random number of asterisks since the password has been hashed and there’s no way back.
Then, the render_template function is returned, passing the dictionary as an argument.
On the other end, the POST method is used when the user is submitting the name of the product he/she wants to delete by click the homonymous button. Hence, request.form.get is used to obtain the user input. It’s relevant to notice though that the user is not obliged to provide a valid input for every field, since he/she could just want to change one or more specifications of the account but not all of them.
Consequently, for every field, the program checks whether it is not blank, and then, if so, runs some more controls (for example if the input has already been used when it comes to the new username or email, or if the new password and its confirmation match), rendering an apology in case of any error occurs. Contrarily, db.execute is invoked to update the user information on the table.
Lastly, the same lines of code as the GET block are used to show the updated user infos.

### INDEX
When the user is logged in, the main page doesn’t display the statistics about food waste (which were meant to show the user the importance of doing something concrete to reduce that serious appalling problem) but the list of products he/she has already inserted, to keep up visually with what he/she has to consume before it goes bad.
Therefore, using the jinja 2 syntax iterating on every item of a list of dictionaries, ordered by expiring date and containing, for every product, its name, expiring date and quantity with the corresponding unit.
To make it more visually appealing, the information of each product is placed in a light grey container, and the name is bolded.
In the #index route, it’s not necessary to make distinctions based on the request method, since it’s only used to display the list of products inserted by the user.
In order to implement that, db.execute is used to select the data from the product table based on the user id, which is equal to the one saved in the session object created by flask. The return value is a list in which every argument is a dictionary containing the information requested about the products. Then, the for cycle iterates over each element of the list, calling the split method to format the expiring date correctly as year-month-date. Lastly, the render template, with the html page and the product list as arguments, is returned.

### ADD
In the html file add.html, the form, styled as a light gray container with a slightly wide border, allows the user can add products to his/her list.
Since multiple informations are needed, I added several divs containing inputs fields, in order:
*the name of the product
*the expiring date, using a specific date input so that the user is presented with a calendar, making the date selection more simple and visually intuitive
*the quantity, using a number input
*the unit of measurement, using a select menu with several options, covering possibly all the units widely used in the world
At the end, there’s the “Add” button the user needs to click to submit the form.
In the #add route, which is accessible only if users have previously logged in, the first if … else permits to distinguish two different blocks of code, depending on the request method. If it is GET, it means the user is visiting the page by clicking on “add” in the navbar, so the add template should be simply render. On the other end, the POST method is used when the user is submitting the informations by click the add button. Consequently, firstly the request.form.get() function must be used to get the data inserted by the user (the name of the product, its expiring date, the quantity and the measurement unit) and store them in the appropriate variables.
However, it is also important to check the inputs, rendering an apology (more on that later) if they were left blank. Moreover, the expiring date is required not to be prior to today’s date. In order to compare them, the inserted expiring date, whose type was string, had to be converted into am actual date. Therefore, a variable (symbolically called dateObject) was created by using the strptime method from the datetime library. It indeed takes two arguments, the string that has to be covered, and a format code (in this case %Y-%m-%d), and returns a date time object. Then, the today() method is used to get the current date. Eventually the two were compared, returning an apology if today was bigger than the expiring date.
If not the case, db.execute is used to insert into the product table the data in the correct column, and the user is rended the add template again, so that he/she can add more products if needed.

### SEARCH
The aim is to enable the user to search products based on their names, which, in my point of view, can be sometimes useful. Imagine this, for example: the user wants some dessert after dinner, so, obviously, he/she can open Just Before and scroll down the whole list of products until she/he finds the first dessert-like item to expire. However, it would be better if the user could just search the name of the dessert he/she wants and be presented with the list of products having that name (or a similar one), ordered by expiring date.
To do that, in the html file, there’s a search bar, whose type is text, where the user can type in the name of the product he/she is looking for, followed by a button to submit it. Below, results are then displayed using jinja 2 syntax. In fact, if the variable passed as argument to the render_template function, called “compatibles”, is not None, every element of “compatibles” is shown, using a for loop, in a grey container.
In the #search route, which is accessible only if users have previously logged in, the first if … else permits to distinguish two different blocks of code, depending on the request method. If it is GET, it means the user is visiting the page by clicking on “search” in the navbar so the page just needs to show the search bar by returning the render_template function without any additional parameter.
On the other end, the POST method is used when the user is submitting the name of the product he/she wants to search for by click the homonymous button.
Thence, request.form.get Is used to get the search name and store it in a variable symbolically called searchedProduct, returning an apology I the field is left blank. Then, a list called compatibles is declared and db.execute is used to select from the table “products” all the products inserted by the user id currently saved in the session object, ordered by expiring date. They are stored in another variable called productsList. Following, inside a for loop iterating on every item of productsList, an if construct checks whether the searched product string is contained in the item name (which is a string itself). If so, db.execute is used to get all the information about that product and, after conveniently formatting the date, they are added to the “compatibles” list. Eventually, the render_template function is returned, passing along the “compatibles” list as argument.

### MAKE CHANGES
The goal with change is to give the user the chance to modify the information regarding a product he/she has already inserted.
Therefore, in the html file, the form consists of:
*a select menu, implemented with jinja syntax, using a for loop to display the names of products already added by the user
*three input fields, to specify if and what data the user want to change between the expiring date, the quantity and the unit (which can be chosen from a select menu)
*a button to click on in order to submit the changes
*a final div containing a message to notify the user if the operation was successful, which, thanks to the jinja if block, appears only after the database has been updated correctly
In the corresponding #change route, which is accessible only if users have previously logged in, the current user id (specified in the session object) is saved in an homonymous variable and db.execute is used to create a list containing all the names of products inserted by the user with such user id. The first if … else permits to distinguish two different blocks of code, depending on the request method. If it is GET, it means the user is visiting the page by clicking on “change” in the navbar so it simply returns the render_template function, passing as arguments  the products list (so that it can be displayed on the webpage) and another variable called selected whose value is set to None (since it is need to display the success message). On the other end, the POST method is used when the user is submitting relevant data by click the “Change” button. Therefore the request.form.get method is used to get the selected product name and the new quantity, unit and expiring date. It’s important to bear in mind that the user is free to change as many fields as he/she wants regarding the product, reason why the following if constructs check on whether the inputs are not blank, without returning any apology if they are though. The only exemption is the product name, which obviously needs to be selected. Hence, for every field that was not left blank, the corresponding row in the “products” table is updated. Eventually the render_template function is returned, passing as arguments the products list (so that it can be displayed on the webpage) and another variable called selected whose value, this time, is the name of the selected product.
Another needed observation is that, unlike in other routes, the productsNames variable is declared and initialised outside the if … else construct and then used in both code blocks. That’s because the list of products among which the user can choose remains the same, regardless of the request method, since the user can always change data about products which he/she has already modified.

### DELETE PRODUCTS
It allows the user to delete a product from the list when it has been eaten or expired, by simply selecting the name in the html page. In fact, using the jinja 2 syntax, products name are presented in alphabetical order as options of a select element, thus making it easier to find and recognise them.
After clicking the delete button, if everything goes well, the user is sent a message of success, thanks to the if block that checks if the value of “selected” (passed as a parameter to the render template function) is equal to None, and, if not, displays the message.

As usual, In the #delete route, which is accessible only if users have previously logged in, the first if … else permits to distinguish two different blocks of code, depending on the request method. If it is GET, it means the user is visiting the page by clicking on “delete” in the navbar, so the user should simply be rendered the delete template. Since the html page consists of a select menu whose options are the products’ name in the database, db.execute is used to select them from the “products” table. The list, stored in a variable called productsName, is sent as a parameter of the render_template function, along with another variable, selected, which is assigned as equal to None, so that any success message is not displayed.
On the other end, the POST method is used when the user is submitting the name of the product he/she wants to delete by click the homonymous button. If so, request.form.get is used to get the name, storing it in a variable called selected. The input is then checked to be sure the user has actually selected an option from the menu. In such case, db.execute is called to delete the product form the table.
Afterwards, db.execute is used again to select the products name, which are eventually sent as a parameter of the render_template function, along with selected, so that the success message can be shown and the user can delete other products if he/she wants to.
The second last line may seem repetitive, since it is used also when the request method is GET, and an observer might think it would have the code shorter to just place the command before, outside the if … else construct. However, that actually would not have worked, because, once the user has deleted a product, it must not be displayed again as an option in the select menu.

### IN HELPERS.PY
There are two functions:
-apology, which has a similar implementation as the one in “Finance”, with the difference that the image used is picturing Boo from Monsters & Co, since I wanted it to be both fun (to make the user laugh at the mistake he made) but representative, at least in my opinion, of something going wrong
-login_required, which I took from “Finance” as well


### STATIC FOLDER
In the static folder, there are all the images I used throughout the website and style.css, in which the elements of the website are added some styling adjustments
In place of the bootstrap default ones. I indeed wanted my website to still be “minimal” but more visually captivating, hence the use of pastel colours for the background and bold and bigger fonts for titles and other important information.


