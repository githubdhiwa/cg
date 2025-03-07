import cv2
import numpy as np
from PIL import Image

# 1. Load and display an image
def load_and_display(path):
    img = cv2.imread(path)
    cv2.imshow("Loaded Image", img)
    return img

# 2. Create random array and store as an image
def create_random_image():
    arr = np.random.randint(0, 256, (256, 256, 3), dtype=np.uint8)
    im = Image.fromarray(arr)
    im.show()
    return np.array(im)

# 3. Resize an image
def resize_image(img, size=(128, 128)):
    return cv2.resize(img, size)

# 4. Rotate an image (general transformation)
def rotate_image(img, angle=45):
    h, w = img.shape[:2]
    M = cv2.getRotationMatrix2D((w//2, h//2), angle, 1.0)
    return cv2.warpAffine(img, M, (w, h))

# 5. Uniform quantization to n gray levels (first convert to grayscale)
def uniform_quantization(img, levels):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    scale = 256 // levels
    quant = (gray // scale) * scale
    return quant

# 6. Generate histogram (returns histogram array)
def generate_histogram(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    hist = cv2.calcHist([gray], [0], None, [256], [0, 256])
    return hist

# 7. Histogram equalization
def hist_equalization(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    return cv2.equalizeHist(gray)

# 8. Contrast stretching (linear normalization)
def contrast_stretching(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    mn, mx = np.min(gray), np.max(gray)
    stretched = ((gray - mn) * (255.0 / (mx - mn))).astype(np.uint8)
    return stretched

# 9. Padding examples: zero, constant, nearest neighbor, reflect
def padding_examples(img):
    pad_zero = cv2.copyMakeBorder(img, 10, 10, 10, 10, cv2.BORDER_CONSTANT, value=[0, 0, 0])
    pad_const = cv2.copyMakeBorder(img, 10, 10, 10, 10, cv2.BORDER_CONSTANT, value=[128, 128, 128])
    pad_nn = cv2.copyMakeBorder(img, 10, 10, 10, 10, cv2.BORDER_REPLICATE)
    pad_reflect = cv2.copyMakeBorder(img, 10, 10, 10, 10, cv2.BORDER_REFLECT)
    return pad_zero, pad_const, pad_nn, pad_reflect

# 10. Spatial filtering: mean, median, max (dilate), min (erode)
def spatial_filters(img):
    mean = cv2.blur(img, (5, 5))
    median = cv2.medianBlur(img, 5)
    max_filter = cv2.dilate(img, np.ones((5, 5), np.uint8))
    min_filter = cv2.erode(img, np.ones((5, 5), np.uint8))
    return mean, median, max_filter, min_filter

# 11. Edge Detection
def edge_detection(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Sobel
    sobelx = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=3)
    sobely = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=3)
    sobel = cv2.magnitude(sobelx, sobely).astype(np.uint8)
    # Prewitt via filter2D
    kernelx = np.array([[1, 0, -1], [1, 0, -1], [1, 0, -1]])
    kernely = np.array([[1, 1, 1], [0, 0, 0], [-1, -1, -1]])
    prewittx = cv2.filter2D(gray, -1, kernelx)
    prewitty = cv2.filter2D(gray, -1, kernely)
    prewitt = cv2.magnitude(np.float32(prewittx), np.float32(prewitty)).astype(np.uint8)
    # Canny
    canny = cv2.Canny(gray, 100, 200)
    # Laplacian
    lap = cv2.Laplacian(gray, cv2.CV_64F)
    lap = np.uint8(np.absolute(lap))
    # Laplacian of Gaussian (LoG)
    log = cv2.GaussianBlur(gray, (3, 3), 0)
    log = cv2.Laplacian(log, cv2.CV_64F)
    log = np.uint8(np.absolute(log))
    return sobel, prewitt, canny, lap, log

# 12. Image Enhancement: power law, log, histogram equalization
def image_enhancement(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gamma = 0.5
    power = np.array(255 * ((gray / 255) ** gamma), dtype='uint8')
    log_trans = np.uint8(255 / np.log(1 + np.max(gray)) * np.log(1 + gray))
    hist_eq = cv2.equalizeHist(gray)
    return power, log_trans, hist_eq

# 13. Morphological Operations: dilation, erosion, opening, closing, hit-or-miss, skeletonization (if available)
def morphological_operations(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    kernel = np.ones((3, 3), np.uint8)
    dilation = cv2.dilate(gray, kernel, iterations=1)
    erosion = cv2.erode(gray, kernel, iterations=1)
    opening = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel)
    closing = cv2.morphologyEx(gray, cv2.MORPH_CLOSE, kernel)
    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)
    hitmiss = cv2.morphologyEx(binary, cv2.MORPH_HITMISS, kernel)
    thinning = None
    if hasattr(cv2, 'ximgproc') and hasattr(cv2.ximgproc, 'thinning'):
        thinning = cv2.ximgproc.thinning(binary)
    return dilation, erosion, opening, closing, hitmiss, thinning

# 14. Fourier Transform and DCT; wavelet transform not available with allowed libraries
def fourier_transform(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    dft = cv2.dft(np.float32(gray), flags=cv2.DFT_COMPLEX_OUTPUT)
    dft_shift = np.fft.fftshift(dft)
    dct = cv2.dct(np.float32(gray))
    return dft_shift, dct

# 15. Convolution filters: Gaussian lowpass, ideal highpass (original - lowpass), Gaussian highpass, bandpass (difference of Gaussians)
def convolution_filters(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gauss_lp = cv2.GaussianBlur(gray, (5, 5), 0)
    ideal_hp = cv2.subtract(gray, gauss_lp)
    gauss_hp = ideal_hp.copy()  # similar effect
    low_sigma = cv2.GaussianBlur(gray, (5, 5), 0)
    high_sigma = cv2.GaussianBlur(gray, (11, 11), 0)
    bandpass = cv2.absdiff(low_sigma, high_sigma)
    return gauss_lp, ideal_hp, gauss_hp, bandpass

# 16. Segmentation: thresholding (histogram based), connected components (region based), edge segmentation
def segmentation(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, thresh = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)
    num_labels, labels = cv2.connectedComponents(thresh)
    edges = cv2.Canny(gray, 100, 200)
    return thresh, labels, edges

# 17. Noise Removal using fastNlMeansDenoising
def noise_removal(img):
    return cv2.fastNlMeansDenoisingColored(img, None, 10, 10, 7, 21)

# Main demonstration of all functions
def main():
    # Load image (change "sample.jpg" to your image path)
    img = load_and_display("sample.jpeg")
    
    # Create and display a random image
    _ = create_random_image()
    
    # Resize and rotate
    resized = resize_image(img)
    rotated = rotate_image(img)
    
    # Uniform quantization for levels 4, 8, 16, 32, 64
    quant_images = {level: uniform_quantization(img, level) for level in [4, 8, 16, 32, 64]}
    
    # Histogram (print first 10 values for brevity)
    hist = generate_histogram(img)
    print("Histogram (first 10 bins):", hist[:10].flatten())
    
    # Histogram equalization and contrast stretching
    heq = hist_equalization(img)
    cs = contrast_stretching(img)
    
    # Padding examples (only showing one example each)
    pad_zero, pad_const, pad_nn, pad_reflect = padding_examples(img)
    
    # Spatial filters
    mean, median, maxf, minf = spatial_filters(img)
    
    # Edge detection methods
    sobel, prewitt, canny, lap, log = edge_detection(img)
    
    # Image enhancement methods
    power, log_trans, hist_eq_enh = image_enhancement(img)
    
    # Morphological operations
    dilation, erosion, opening, closing, hitmiss, thinning = morphological_operations(img)
    
    # Fourier Transform and DCT
    dft_shift, dct = fourier_transform(img)
    
    # Convolution filters
    gauss_lp, ideal_hp, gauss_hp, bandpass = convolution_filters(img)
    
    # Segmentation
    thresh, labels, edges = segmentation(img)
    
    # Noise removal
    denoised = noise_removal(img)
    
    # Display a few results for demonstration
    cv2.imshow("Resized", resized)
    cv2.imshow("Rotated", rotated)
    cv2.imshow("Histogram Equalized", heq)
    cv2.imshow("Contrast Stretched", cs)
    cv2.imshow("Canny Edge Detection", canny)
    cv2.imshow("Denoised", denoised)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
