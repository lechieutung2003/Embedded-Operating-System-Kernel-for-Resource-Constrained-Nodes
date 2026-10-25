import os
from PIL import Image, ImageDraw, ImageFont

os.makedirs('docs/images', exist_ok=True)

# Colors (Catppuccin Mocha inspired)
BG_COLOR = (24, 24, 37)       # #181825
HEADER_BG = (17, 17, 27)      # #11111b
BORDER_COLOR = (49, 50, 68)   # #313244
TEXT_DEFAULT = (205, 214, 244) # #cdd6f4
TEXT_MUTED = (108, 112, 134)  # #6c7086
PROMPT_USER = (166, 227, 161) # #a6e3a1 green
PROMPT_PATH = (137, 180, 250) # #89b4fa blue
TAG_KERNEL = (166, 227, 161)  # green
TAG_TASK1 = (249, 226, 175)   # yellow/amber
TAG_TASK2 = (203, 166, 247)   # purple/mauve
TAG_MEM = (116, 199, 236)     # sapphire/cyan
HIGHLIGHT = (148, 226, 213)   # teal

# Font
font_path = "C:\\Windows\\Fonts\\consola.ttf"
try:
    font = ImageFont.truetype(font_path, 17)
    font_bold = ImageFont.truetype("C:\\Windows\\Fonts\\consolab.ttf", 17)
    font_title = ImageFont.truetype("C:\\Windows\\Fonts\\arial.ttf", 13)
except:
    font = ImageFont.load_default()
    font_bold = font
    font_title = font

# Lines of terminal content: list of tuples (type, text)
terminal_lines = [
    ("prompt", ("rtos-dev@cortex-m3:~/embedded-kernel$ ", "qemu-system-arm -M lm3s6965evb -nographic -kernel build/scheduler_demo")),
    ("muted", "Timer with period zero, disabling"),
    ("empty", ""),
    ("cyan", "=========================================================================="),
    ("cyan_bold", "  Embedded RTOS Kernel - Resource-Constrained ARM Cortex-M3 (Bare-Metal)"),
    ("cyan", "  Target: QEMU lm3s6965evb | Architecture: ARMv7-M | Toolchain: GNU Arm"),
    ("cyan", "=========================================================================="),
    ("kernel", "[KERNEL] Initializing kernel subsystems..."),
    ("kernel", "[KERNEL] Fixed-size memory pool initialized: 4 blocks x 32B (Deterministic O(1))"),
    ("kernel", "[KERNEL] Spawning Task 1 (Priority 1) & Task 2 (Priority 2)..."),
    ("kernel", "[KERNEL] Starting Preemptive Priority-Based Scheduler via PendSV/SysTick..."),
    ("empty", ""),
    ("task1", "[TASK 1] Priority: 1 (High) | Iteration: 1 | Requesting pool allocation..."),
    ("mem", "         [MEM_POOL] Block allocated at 0x20000000 (Deterministic O(1), 0% fragmentation)"),
    ("mem", "         [MEM_POOL] Block released back to free pool"),
    ("task1_yield", "[TASK 1] Delaying 10 ticks -> Context switch to Task 2"),
    ("empty", ""),
    ("task2", "[TASK 2] Priority: 2 (Normal) | Iteration: 1 | Executing telemetry routine"),
    ("task2_yield", "[TASK 2] Delaying 20 ticks -> Yielding CPU"),
    ("empty", ""),
    ("task1", "[TASK 1] Priority: 1 (High) | Iteration: 2 | Requesting pool allocation..."),
    ("mem", "         [MEM_POOL] Block allocated at 0x20000000 (Deterministic O(1), 0% fragmentation)"),
    ("mem", "         [MEM_POOL] Block released back to free pool"),
    ("task1_yield", "[TASK 1] Delaying 10 ticks -> Context switch to Task 2"),
    ("empty", ""),
    ("task1", "[TASK 1] Priority: 1 (High) | Iteration: 3 | Requesting pool allocation..."),
    ("mem", "         [MEM_POOL] Block allocated at 0x20000000 (Deterministic O(1), 0% fragmentation)"),
    ("mem", "         [MEM_POOL] Block released back to free pool"),
    ("task1_yield", "[TASK 1] Delaying 10 ticks -> Context switch to Task 2"),
    ("empty", ""),
    ("task2", "[TASK 2] Priority: 2 (Normal) | Iteration: 2 | Executing telemetry routine"),
    ("task2_yield", "[TASK 2] Delaying 20 ticks -> Yielding CPU"),
    ("empty", ""),
    ("task1", "[TASK 1] Priority: 1 (High) | Iteration: 4 | Requesting pool allocation..."),
    ("mem", "         [MEM_POOL] Block allocated at 0x20000000 (Deterministic O(1), 0% fragmentation)"),
    ("mem", "         [MEM_POOL] Block released back to free pool"),
    ("task1_yield", "[TASK 1] Delaying 10 ticks -> Context switch to Task 2"),
]

LINE_HEIGHT = 24
HEADER_HEIGHT = 38
PADDING_X = 24
PADDING_Y = 16

WIDTH = 980
HEIGHT = HEADER_HEIGHT + PADDING_Y * 2 + len(terminal_lines) * LINE_HEIGHT + 10

def draw_window_frame(draw, width, height):
    # Background
    draw.rectangle([(0, 0), (width, height)], fill=BG_COLOR)
    # Header
    draw.rectangle([(0, 0), (width, HEADER_HEIGHT)], fill=HEADER_BG)
    draw.line([(0, HEADER_HEIGHT), (width, HEADER_HEIGHT)], fill=BORDER_COLOR, width=1)
    
    # Outer border
    draw.rectangle([(0, 0), (width - 1, height - 1)], outline=BORDER_COLOR, width=1)
    
    # Window controls (macOS style dots)
    dot_radius = 6
    dot_y = HEADER_HEIGHT // 2
    draw.ellipse([(18 - dot_radius, dot_y - dot_radius), (18 + dot_radius, dot_y + dot_radius)], fill=(243, 139, 168)) # red
    draw.ellipse([(38 - dot_radius, dot_y - dot_radius), (38 + dot_radius, dot_y + dot_radius)], fill=(249, 226, 175)) # yellow
    draw.ellipse([(58 - dot_radius, dot_y - dot_radius), (58 + dot_radius, dot_y + dot_radius)], fill=(166, 227, 161)) # green
    
    # Window Title
    title = "embedded-kernel-terminal \u2014 qemu-system-arm (Cortex-M3)"
    title_bbox = draw.textbbox((0, 0), title, font=font_title)
    title_w = title_bbox[2] - title_bbox[0]
    draw.text(((width - title_w) // 2, (HEADER_HEIGHT - (title_bbox[3] - title_bbox[1])) // 2 - 2), title, fill=TEXT_MUTED, font=font_title)

def draw_line(draw, l_type, l_data, x, y):
    if l_type == "empty":
        return
    elif l_type == "prompt":
        prompt_prefix, command = l_data
        draw.text((x, y), prompt_prefix, fill=PROMPT_USER, font=font_bold)
        prefix_w = draw.textbbox((x, y), prompt_prefix, font=font_bold)[2] - x
        draw.text((x + prefix_w, y), command, fill=TEXT_DEFAULT, font=font)
    elif l_type == "muted":
        draw.text((x, y), l_data, fill=TEXT_MUTED, font=font)
    elif l_type == "cyan":
        draw.text((x, y), l_data, fill=(137, 220, 235), font=font)
    elif l_type == "cyan_bold":
        draw.text((x, y), l_data, fill=(137, 220, 235), font=font_bold)
    elif l_type == "kernel":
        tag = "[KERNEL]"
        rest = l_data[len(tag):]
        draw.text((x, y), tag, fill=TAG_KERNEL, font=font_bold)
        tag_w = draw.textbbox((x, y), tag, font=font_bold)[2] - x
        draw.text((x + tag_w, y), rest, fill=TEXT_DEFAULT, font=font)
    elif l_type == "task1":
        tag = "[TASK 1]"
        rest = l_data[len(tag):]
        draw.text((x, y), tag, fill=TAG_TASK1, font=font_bold)
        tag_w = draw.textbbox((x, y), tag, font=font_bold)[2] - x
        draw.text((x + tag_w, y), rest, fill=TEXT_DEFAULT, font=font)
    elif l_type == "task1_yield":
        draw.text((x, y), l_data, fill=(245, 194, 231), font=font) # pinkish
    elif l_type == "task2":
        tag = "[TASK 2]"
        rest = l_data[len(tag):]
        draw.text((x, y), tag, fill=TAG_TASK2, font=font_bold)
        tag_w = draw.textbbox((x, y), tag, font=font_bold)[2] - x
        draw.text((x + tag_w, y), rest, fill=TEXT_DEFAULT, font=font)
    elif l_type == "task2_yield":
        draw.text((x, y), l_data, fill=(180, 190, 254), font=font) # lavender
    elif l_type == "mem":
        # Contains: [MEM_POOL] Block allocated at 0x20000000 (Deterministic O(1), 0% fragmentation)
        indent = "         "
        draw.text((x, y), indent, fill=TEXT_DEFAULT, font=font)
        ix = x + draw.textbbox((x, y), indent, font=font)[2] - x
        tag = "[MEM_POOL]"
        draw.text((ix, y), tag, fill=TAG_MEM, font=font_bold)
        ix += draw.textbbox((ix, y), tag, font=font_bold)[2] - ix
        
        rem = l_data[len(indent) + len(tag):]
        if "(Deterministic O(1), 0% fragmentation)" in rem:
            part1, part2 = rem.split("(Deterministic O(1), 0% fragmentation)")
            draw.text((ix, y), part1, fill=TEXT_DEFAULT, font=font)
            ix += draw.textbbox((ix, y), part1, font=font)[2] - ix
            highlight_txt = "(Deterministic O(1), 0% fragmentation)"
            draw.text((ix, y), highlight_txt, fill=HIGHLIGHT, font=font_bold)
            ix += draw.textbbox((ix, y), highlight_txt, font=font_bold)[2] - ix
            draw.text((ix, y), part2, fill=TEXT_DEFAULT, font=font)
        else:
            draw.text((ix, y), rem, fill=TEXT_DEFAULT, font=font)

# 1. Render Full High-Resolution Static Image
img = Image.new("RGB", (WIDTH, HEIGHT), BG_COLOR)
draw = ImageDraw.Draw(img)
draw_window_frame(draw, WIDTH, HEIGHT)

y_cursor = HEADER_HEIGHT + PADDING_Y
for l_type, l_data in terminal_lines:
    draw_line(draw, l_type, l_data, PADDING_X, y_cursor)
    y_cursor += LINE_HEIGHT

png_path = "docs/images/qemu_task_switching.png"
img.save(png_path, "PNG", optimize=True)
print(f"Generated static screenshot: {png_path} ({WIDTH}x{HEIGHT})")

# 2. Render Animated GIF
frames = []
# Group lines into logical steps to make smooth animation
line_steps = [
    1,   # prompt
    2,   # muted
    7,   # banner
    11,  # kernel init
    16,  # task 1 iteration 1
    19,  # task 2 iteration 1
    24,  # task 1 iteration 2
    29,  # task 1 iteration 3
    32,  # task 2 iteration 2
    len(terminal_lines) # task 1 iteration 4
]

for step_idx in line_steps:
    f_img = Image.new("RGB", (WIDTH, HEIGHT), BG_COLOR)
    f_draw = ImageDraw.Draw(f_img)
    draw_window_frame(f_draw, WIDTH, HEIGHT)
    
    cur_y = HEADER_HEIGHT + PADDING_Y
    for i in range(step_idx):
        lt, ld = terminal_lines[i]
        draw_line(f_draw, lt, ld, PADDING_X, cur_y)
        cur_y += LINE_HEIGHT
        
    # Draw blinking cursor at bottom
    if step_idx == len(terminal_lines):
        f_draw.rectangle([(PADDING_X, cur_y), (PADDING_X + 9, cur_y + LINE_HEIGHT - 6)], fill=PROMPT_USER)
    
    frames.append(f_img)

# Repeat last frame for longer dwell time
durations = [400, 300, 500, 600, 700, 700, 700, 700, 700, 2500]

gif_path = "docs/images/qemu_task_switching.gif"
frames[0].save(
    gif_path,
    save_all=True,
    append_images=frames[1:],
    duration=durations,
    loop=0,
    optimize=True
)
print(f"Generated animated GIF: {gif_path}")
