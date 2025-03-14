1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_The remote client uses the EOF character to figure out when a command has fully been received, and continues to recv in a loop until this is found (or another error occurs). As long as wehave a loop that continues to run until breaking due to an EOF, we will ensure complete message transmission._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_We should use things like EOF characters or perhaps newlines to detect the ends of commands, and if this is not handled correctly we may parse little chunks of commands. I had a bug like this for a while where my server would only read 8 characters at a time and then queue the rest of the command as a different nonsense command._

3. Describe the general differences between stateful and stateless protocols.

_Most simply, stateful protocols track information between different client requests, so that broader client-to-client communication and interaction, or just saving progress, is possible. Stateless protocols do not do this, so each client starts with no data based on any others._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_UDP might be useful in a scenario where perfection isn't required and we don't necessarily need to check for lots of errors or make sure we read every byte, like if we're sending a lot of video footage for example and some artefacts in it are fine and expected._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_The operating system provides sockets for network communications for application use._