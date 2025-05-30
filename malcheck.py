# Malware checker using VirusTotal by Nathna W Jones nat@davaosecurity.com for ECE Credits
# script uploads the file to VirusTotal and waits for a short period before retrieving the analysis report. 
# Depending on the file size and VirusTotal's processing time, you may need to adjust the sleep duration.
# pip install requests
# sign up for a VirusTotal account: You need an API key to access the VirusTotal service.
# Replace YOUR_API_KEY: Make sure to replace 'YOUR_API_KEY' in the script with your actual VirusTotal API key.
# script will upload the specified file to VirusTotal, wait for the analysis to complete, retrieve the report, and save it as a JSON file in the same directory as the input file. The report file will be named <original_file_name>_vt_report.json.
# python3 vt_malware_analysis.py /path/to/your/file

import requests
import time
import sys
import json

# Replace 'YOUR_API_KEY' with your actual VirusTotal API key
API_KEY = 'YOUR_API_KEY'
VT_URL = 'https://www.virustotal.com/api/v3/files'

def upload_file(file_path):
    """Upload a file to VirusTotal for analysis."""
    headers = {
        'x-apikey': API_KEY
    }

    with open(file_path, 'rb') as file:
        response = requests.post(VT_URL, headers=headers, files={'file': file})

    if response.status_code == 200:
        return response.json()['data']['id']
    else:
        print(f"Error uploading file: {response.json()}")
        sys.exit(1)

def get_report(file_id):
    """Retrieve the analysis report from VirusTotal."""
    headers = {
        'x-apikey': API_KEY
    }

    report_url = f"{VT_URL}/{file_id}"
    response = requests.get(report_url, headers=headers)

    if response.status_code == 200:
        return response.json()
    else:
        print(f"Error retrieving report: {response.json()}")
        sys.exit(1)

def save_report(report, output_file):
    """Save the VirusTotal report to a local file."""
    with open(output_file, 'w') as file:
        json.dump(report, file, indent=4)
    print(f"Report saved to {output_file}")

def main(file_path):
    """Main function to analyze a file for malware."""
    print(f"Uploading file: {file_path}")
    file_id = upload_file(file_path)
    print(f"File uploaded. ID: {file_id}")

    # Wait for a few seconds to allow analysis to complete
    print("Waiting for analysis to complete...")
    time.sleep(15)  # Adjust the sleep time as needed

    print("Retrieving report...")
    report = get_report(file_id)

    # Save the report to a local file
    output_file = f"{file_path}_vt_report.json"
    save_report(report, output_file)

    # Print a summary of the report
    print("Report Summary:")
    print(f"File ID: {report['data']['id']}")
    print(f"File Name: {report['data']['attributes']['name']}")
    print(f"Malicious: {report['data']['attributes']['last_analysis_stats']['malicious']} / {report['data']['attributes']['last_analysis_stats']['undetected']}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 vt_malware_analysis.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    main(file_path)
