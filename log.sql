-- Keep a log of any SQL queries you execute as you solve the mystery.
--reading crime scene report
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";
--transcript of interviews
SELECT name, transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;
--table called suspects where I inserted the names of people on the bakery security logs
CREATE TABLE suspects AS
SELECT people.name, people.id, people.passport_number, people.phone_number
FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute < 25
AND activity = "exit";
--finding the only person making a transaction in Leggett Street that very day >>Amanda
CREATE TABLE evenmore AS
SELECT suspects.name FROM suspects
JOIN people ON suspects.name = people.name
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.transaction_type = "withdraw";
--who between evenmore did a phone call which lasted less than 60 seconds
CREATE TABLE last AS
SELECT evenmore.name FROM evenmore
JOIN people ON evenmore.name = people.name
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
--id of the first next flight from fiftyville >>36
SELECT id, hour, minute FROM flights
WHERE year = 2021 AND month = 7 AND day = 29
AND flights.origin_airport_id =
(SELECT id FROM airports WHERE city = "Fiftyville")
ORDER BY hour, minute ASC;
--destination >> NEW YORK CITY
SELECT city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE flights.id = 36;
--who took that plane
SELECT name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE flights.id = 36;
--whom Bruce called >>Robin
SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE caller = (SELECT people.phone_number FROM people WHERE name = "Bruce")
AND year = 2021 AND month = 7 AND day = 28 AND duration < 60;
