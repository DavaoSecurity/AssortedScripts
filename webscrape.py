# Basic Rootkit hunter by Nathan W Jones nat@davaosecurity.com for ECE Credits
# web scraper that gets all details about the web server, web app and OS. Output to a formatted HTML file
# Requests Library: Used to send HTTP requests and receive responses.
# BeautifulSoup: Used to parse the HTML content of the web page.
# Socket Library: Used to resolve the hostname to an IP address.
# HTML Output: The script generates an HTML file with a table displaying the server information.
# File Writing: The HTML content is written to a file named server_info.html.
# pip install requests beautifulsoup4


import requests
from bs4 import BeautifulSoup
import socket

def get_server_info(url):
    try:
        # Send a GET request to the URL
        response = requests.get(url)

        # Get server details from the response headers
        server = response.headers.get('Server', 'Unknown')
        x_powered_by = response.headers.get('X-Powered-By', 'Unknown')

        # Parse the HTML content
        soup = BeautifulSoup(response.content, 'html.parser')

        # Extract web application details (if available)
        app_details = soup.find('meta', attrs={'name': 'generator'})
        app_name = app_details['content'] if app_details else 'Unknown'

        # Get the IP address and hostname
        hostname = socket.gethostbyname(url.split("//")[-1].split("/")[0])

        # Create HTML output
        html_content = f"""
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Server Information</title>
            <style>
                body {{ font-family: Arial, sans-serif; margin: 20px; }}
                h1 {{ color: #333; }}
                table {{ width: 100%; border-collapse: collapse; margin-top: 20px; }}
                th, td {{ border: 1px solid #ddd; padding: 8px; text-align: left; }}
                th {{ background-color: #f2f2f2; }}
            </style>
        </head>
        <body>
            <h1>Server Information for {url}</h1>
            <table>
                <tr>
                    <th>Detail</th>
                    <th>Value</th>
                </tr>
                <tr>
                    <td>Server</td>
                    <td>{server}</td>
                </tr>
                <tr>
                    <td>X-Powered-By</td>
                    <td>{x_powered_by}</td>
                </tr>
                <tr>
                    <td>Web Application</td>
                    <td>{app_name}</td>
                </tr>
                <tr>
                    <td>Hostname/IP</td>
                    <td>{hostname}</td>
                </tr>
            </table>
        </body>
        </html>
        """

        # Write the HTML content to a file
        with open('server_info.html', 'w') as file:
            file.write(html_content)

        print("Server information has been written to 'server_info.html'.")

    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
url = "http://example.com"  # Replace with the target URL
get_server_info(url)
