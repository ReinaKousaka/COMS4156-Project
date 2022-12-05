# project-tiger
## About
Project Tiger will offer data analytics for games and competitions. These analytics will be between records/milestones/statistics for each user that clients will host to general analytics regarding total user counts per day, week, month, etc. , most common winning strategies, and general overall user results. Basically, our server provides analytics for our clients to have access to.

The clients will be organizations that host games and/or competitions. Basically, any client that desires analytics regarding competitive play and milestone tracking will be able to utilize our API. Our clients will be allowed to display this data for its users and they can decide which data to showcase publicly, privately per each of their users, or retain internally for research purposes. With regards to research purposes, our clients could be able to use information we provide to make decisions as to how to further develop their applications.

The data we will be accumulating will be records/milestones per user (such as best result achieved, biggest bet won, biggest bet lost, total number of games, win-loss ratio). In order to keep track of these records, our server will store the username of all the users that our clients have. With regards to created data, our server can calculate the win-loss ratio and predict best possible move suggestions depending on which game is being played. The other forms of data are game logs, which contain metadata such as number of players, player ids, moves made, and money wagered. These game logs will be made accessible to all of our clients’ users to search for with game ids.


## Authentication and API rationale
We use a token-based approach to authentication where each token uniquely identifies an account and allows it to upload data about games and retrieve statistics for the given account. Since each account has one access token, our service is stateless and does not need to have a separate login function. On each request the token is used to authenticate by looking up the corresponding account ID or returning an error if there is no account for the given token. The server then uses the account ID to upload data or compute statistics based on the data for the account ID, i.e. the client can only make changes for the given account ID it is authorized for as inferred from the token. As long as the tokens are secret, clients have no way to read or write the data of other accounts.

The implementation uses HTTPS and POST requests as supported by the Crow REST API and OpenSSL for SSL, SHA-256 hashing and cryptographically secure pseudorandom data, which allows us to base authentication and authorization on established open-source libraries.

In production use, clients using the service would be provided an access token off-channel and would use it to access data based on the internal account ID. For demonstration purposes, clients can request a new token and associated account ID to be sent to them using the `/create_account` API, which allows us to add new clients to effectively debug, test and showcase the server.

Each request that requires authentication takes a token parameter within the body of a POST request alongside any other parameters. For example, an upload request might have a body of `token=<token as returned by server>&gametype=chess&result=p1_wins&earning=100&player=p1`. Similarly, requests for private statistics may include a player or gametype parameter as elaborated below. We also provide a number of global public statistics across all accounts, which can be accessed using GET requests without requiring a token.

For clients using our interface, commands such as `help` and `help-private` provide the full list of supported endpoints for both public and private statistics.

## API reference
The server can be accessed on port 18080 using the following API. At present, all API methods are implemented using HTTP GET requests using CROW on the server side and curl for the example clients.

```/``` URL used to test that the interface works, returns "Welcome to Project Tiger" if the server is working.

```/create_account``` GET request to create and return a pseudorandom unique token for the client to save as an authentication key. The token is hashed using SHA-256 and added to a table alongside an autogenerated account id. Does not require authentication.

```/get_account_id``` POST request for autogenerated account id associated with token. Intended for debug purposes; the client has no need (but also no malicious uses) for the account id.

```/upload``` POST request to upload game data to server, parameters = ```{ gametype: <string>, result: <string>, earning: <int>, player: <string> }```. Player stats are auto updated based on result. Token is used for authentication and game data is sent via request body.

```/public/<type:<string> ['total-games', 'total-players', 'total-types']>``` Request public game data of a given type. Returns an error if the type is invalid or there is an error when querying the data.GET request, no authentication or parameters required.

```private/total-earnings-all``` POST request for total earnings for all players for a client. Token is used for authentication and querying data.

```private/total-earnings-game``` POST request for total earnings for a specific game for a client. Token is used for authentication and `gametype` is used for querying data.

```private/total-earnings-player``` POST request for total earnings for a specific player for a client. Token is used for authentication and `player` is used for querying data.

```private/total-wins-all``` POST request for total wins for all players for a client. Token is used for authentication and querying data.

```private/total-wins-game``` POST request for total wins for a specific game for a client. Token is used for authentication and `gametype` is used for querying data.

```private/total-wins-player``` POST request for total wins for a specific player for a client. Token is used for authentication and `player` is used for querying data.

```private/total-losses-all``` POST request for total losses for all players for a client. Token is used for authentication and querying data.

```private/total-losses-game``` POST request for total losses for a specific game for a client. Token is used for authentication and `gametype` is used for querying data.

```private/total-losses-player``` POST request for total losses for a specific player for a client. Token is used for authentication and `player` is used for querying data.

```private/total-players-for-game``` POST request for total players for a specific game the client created. Token is used for authentication and `gametype` is used for querying data.

```private/number-of-games``` POST request for total number of games for a specific game the client created. Token is used for authentication and `gametype` is used for querying data.

```private/number-of-players``` POST request for total players for across all games the client created. Token is used for authentication and querying data.

```private/greatest-player-by-wins``` POST request for a specific player from a specific game type the client created. Token is used for authentication and querying data.

```private/most-common-play``` POST request for most common result for a specific game created by the client. Token is used for authentication and `gametype` is used for querying data.

```private/most-winning-play``` POST request for most common winning result for a specific game created by the client. Token is used for authentication and `gametype` is used for querying data.

```private/total-player-earning-for-game``` POST request for total earnings for a specific game for a specific player. Token is used for authentication and `gametype` and `player` are used for querying data.

```private/total-player-debt-for-game``` POST request for total money lost for a specific game for a specific player. Token is used for authentication and `gametype` and `player` are used for querying data.

```private/total-player-wins-for-game``` POST request for total wins for a specific game for a specific player. Token is used for authentication and `gametype` and `player` are used for querying data.

```private/total-player-losses-for-game``` POST request for total losses for a specific game for a specific player. Token is used for authentication and `gametype` and `player` are used for querying data.

```private/most-winning-player-for-game``` POST request for player with the most wins for a specific game. Token is used for authentication and `gametype` is used for querying data.

```/private/median-earning``` POST request for median money earned for a specific game. Token is used for authentication and `gametype` is used for querying data.

### Build directions
```
#install required libraries
sudo apt install --no-install-recommends -y libasio-dev libcurl4-openssl-dev
pip install gcovr

#set up build folder
mkdir build
cd build
cmake ..

#build server and client
make server client

#build tests
make tiger-test

```

### Run program, analysis tools and tests
```
#Launch server
./server

#Run example client, provides interface to run commands and talk with server
./client

#Build and run all {unit, system, integration} tests
make run-tiger-test

#Generate code coverage results
make coverage

#Run end-to-end tests
make end-end-tests

#Run linter, spell checker and static analysis
#Make sure the tools described below are available
make code-lint
make code-analyze
make code-spell
```

## Libraries

#### curl, curlpp, asio
The program uses [libcurl](https://curl.se/libcurl/), [curlpp](https://github.com/jpbarrette/curlpp) and [asio](https://think-async.com/Asio/) for the network code. It assumes that asio and curl are installed while curlpp is included in src/libraries/curlpp. The libraries are used in the client and server programs and are available under the Curl License, MIT licence and Boost Software License, respectively.

#### SQLite
[SQLite](https://www.sqlite.org/) is available in the src/libraries/sqlite folder. In its current form, it is the amalgamated source (i.e. all source files merged, which is the preferred distribution). SQLite is in the public domain. We use SQLite for the database class used by the server.

#### Crow
[Crow](https://github.com/CrowCpp/Crow) is available in src/libraries/Crow. It is C++ framework for HTTP and Websocket web services with routing for the server. It is covered by a BSD-style license.

#### Openssl
[Openssl](https://github.com/janbar/openssl-cmake) is required by curl and Crow as well as being used as our authentication library for generating random tokens as well as computing SHA-256 hashes. It should come installed on Linux and is not included in this repository. It is available under the OpenSSL License.

## Testing/Mocking/Coverage
We use [GoogleTest and GMock](https://github.com/google/googletest), available in test/googletest. The libraries use a 3-Clause BSD license. Use `make run-tiger-test` to run the tests.

#### Gcovr
[Gcovr](https://gcovr.com/en/stable/) is a utility for managing the use of the GNU gcov utility and generating summarized code coverage results. Gcovr is available under a 3-clause BSD License and can be installed using `pip install gcovr` . Use `make coverage` to generate coverage report, you may also execute `Open ./coverage/index.html` to view the report in your browser.

## Tools for linting, static analysis and spelling
#### Cpplint
We will be using cpplint, which enforces the [google style guide](https://google.github.io/styleguide/cppguide.html). To install, run `pip3 install cpplint`.

Note that Google's style guide is pretty restrictive, so we decided to disable some of the warnings, in particular for GNU extensions and some C-style constructs.

Use `make code-lint` to run the linter on our code base (excluding third-party libraries)

#### Cppcheck (static analysis)
Install [cppcheck](https://cppcheck.sourceforge.io/) and run `cppcheck --enable=warning,style,unusedFunction <path to repo>`. For this project, just run `make code-analyze`.

#### Codespell (spelling)
Install [codespell](https://github.com/codespell-project/codespell) using `pip3 install codespell` and run `codespell <path to repo>` to find spelling mistakes (or `make code-spell`).

##Example client and developing third-party clients
For testing, the example client can be built with `make client` and run using ./client. The client requests a token from the server and stores it in a file for persistence when the client is used later on. For demonstration purposes, the token path can also be provided as a parameter, e.g. `./client client-token-alt.dat`. The client has a command line interface which makes it possible to send commands and upload data or receive the requested statistics from the server. The command line interface exposes the entire API except for the /create_account and /get_account_id routes, which are not needed once a token is available.

Using the provided documentation, a third party will be able to develop a similar client – since our approach builds on widely used web technologies, a third party only needs to get a token, either through another channel or the `/create_token` interface and then use the token to send requests to upload game data or request the various statistics.

## Testing
Our systems, unit and integration tests can be found in the `test` folder and use mocking as needed. Use `make run-tiger-test` to run the tests.

The systems tests are located in `test/serverApiTests.cpp` and use the mocking features of the Crow REST library which allow us to set up a server without actually running it. We then use the handle_full() function to send requests to the server and compare the responses with the expected responses using GTest. This allows us to test the server API directly without any external tools.

## End-to-End testing
For end-to-end testing, we use the script in `test/end-end/end-end.sh` which can be executed with `make end-end-tests` for convenience. The script starts the server, runs the client with a list of commands, captures the client output and compares it with the expected output. A test failure of end-end-tests indicates that the output produced by the client did not match the expected output. Since the client executes commands that cover a wide range of server functionality, this allows us to conduct comprehensive end-to-end testing. In particular, our end-to-end tests demonstrate the persistence and multiple clients features of the server. See `test/end-end` for details.

Since the end-to-end tests depend on client and server, they are included in the CI build job for convenience and quicker CI runs.

## Continuous Integration
We use GitHub Actions for CI integration. The configuration file can be found at `.github/workflows/cmake.yml`. CI runs on every pull request and commit to the trunk. On CI, we have separate jobs to build the client and server, running the tests and providing code coverage reports, linting, and static analysis. The relevant output of each job can generally be found in the penultimate step or the one marked as print. The jobs are also set up to fail if there are any test or build failures or a non-negligible number of lint/static analysis issues, which has been been useful for identifying issues early.

The CI reports are located on the Actions tab of the repository. **Please find the latest tests output as well as spelling, linter/style checker, static analysis and coverage reports in the CI results.** Note that we have tentatively fixed all or almost all spelling, linting and static analysis issues, so you may only see the file names being listed within the output as the program moves through them.

