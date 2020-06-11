--list the names of all people who starred in a movie released in 2004, ordered by birth year
SELECT name FROM people
WHERE
id IN
(SELECT DISTINCT people.id FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON stars.movie_id = movies.id
WHERE
year = 2004)
ORDER BY people.birth ASC;





