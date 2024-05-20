from seleniumwire import webdriver
from selenium.webdriver.firefox.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import gzip

# script kayekhdem gha f linux darori t installe geckodriver
def spin_please():
    try:
        service = Service('/usr/bin/geckodriver')
        driver = webdriver.Firefox(service=service)
        while True:
            driver.get("https://ma.oraimo.com/promotion/ma/freepods-sale/index.html")
            wait = WebDriverWait(driver, 10)
            arrow_button = wait.until(EC.element_to_be_clickable((By.XPATH, '//*[@id="arrow"]')))
            arrow_button.click()
            layer = wait.until(EC.presence_of_element_located((By.XPATH, '/html/body/div[4]')))
            # ila taye9 fel wifi dyalek bla maten3ess machi philo hadi
            # time.sleep(5)
            for request in driver.requests:
                if "luck.php" in request.url:
                    with open("response.txt", "a") as file:
                        decompressed_data = gzip.decompress(request.response.body)
                        file.write(decompressed_data.decode('utf-8') + "\n")
            driver.refresh()
    except:
        print(f"f**k oraimo 1")
    finally:
        driver.quit()
try:
        spin_please()
except:
        print(f"f**k oraimo 2")
