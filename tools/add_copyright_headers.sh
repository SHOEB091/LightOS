#!/bin/bash
# Script to add copyright headers to source files

# Define colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Copyright header content
COPYRIGHT_HEADER=$(cat <<'EOF'
/**
 * LightOS - A Fast, Resource-Efficient Linux-Based Operating System
 * Copyright (c) 2023-2024 LightOS
 * 
 * This file is part of LightOS.
 * 
 * LightOS is free software: you can redistribute it and/or modify
 * it under the terms of the MIT License as published in the LICENSE file.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * MIT License for more details.
 */
EOF
)

# Assembly copyright header
ASM_COPYRIGHT_HEADER=$(cat <<'EOF'
; LightOS - A Fast, Resource-Efficient Linux-Based Operating System
; Copyright (c) 2023-2024 LightOS
; 
; This file is part of LightOS.
; 
; LightOS is free software: you can redistribute it and/or modify
; it under the terms of the MIT License as published in the LICENSE file.
; 
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; MIT License for more details.
EOF
)

# Shell script copyright header
SHELL_COPYRIGHT_HEADER=$(cat <<'EOF'
#!/bin/bash
# LightOS - A Fast, Resource-Efficient Linux-Based Operating System
# Copyright (c) 2023-2024 LightOS
# 
# This file is part of LightOS.
# 
# LightOS is free software: you can redistribute it and/or modify
# it under the terms of the MIT License as published in the LICENSE file.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# MIT License for more details.
EOF
)

# Makefile copyright header
MAKEFILE_COPYRIGHT_HEADER=$(cat <<'EOF'
# LightOS - A Fast, Resource-Efficient Linux-Based Operating System
# Copyright (c) 2023-2024 LightOS
# 
# This file is part of LightOS.
# 
# LightOS is free software: you can redistribute it and/or modify
# it under the terms of the MIT License as published in the LICENSE file.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# MIT License for more details.
EOF
)

# Function to check if file already has a copyright header
has_copyright_header() {
    local file=$1
    grep -q "Copyright (c)" "$file"
    return $?
}

# Function to add copyright header to a file
add_header() {
    local file=$1
    local header=$2
    local temp_file=$(mktemp)
    
    echo "$header" > "$temp_file"
    echo "" >> "$temp_file"
    cat "$file" >> "$temp_file"
    mv "$temp_file" "$file"
    echo -e "${GREEN}Added copyright header to:${NC} $file"
}

# Process C/C++/H files
process_c_files() {
    local count=0
    local skipped=0
    
    echo -e "${YELLOW}Processing C/C++/H files...${NC}"
    
    find . -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -not -path "*/\.*" | while read file; do
        if has_copyright_header "$file"; then
            skipped=$((skipped + 1))
        else
            add_header "$file" "$COPYRIGHT_HEADER"
            count=$((count + 1))
        fi
    done
    
    echo -e "${GREEN}Added headers to $count C/C++/H files. Skipped $skipped files that already had headers.${NC}"
}

# Process Assembly files
process_asm_files() {
    local count=0
    local skipped=0
    
    echo -e "${YELLOW}Processing Assembly files...${NC}"
    
    find . -type f \( -name "*.asm" -o -name "*.s" \) -not -path "*/\.*" | while read file; do
        if has_copyright_header "$file"; then
            skipped=$((skipped + 1))
        else
            add_header "$file" "$ASM_COPYRIGHT_HEADER"
            count=$((count + 1))
        fi
    done
    
    echo -e "${GREEN}Added headers to $count Assembly files. Skipped $skipped files that already had headers.${NC}"
}

# Process Shell script files
process_shell_files() {
    local count=0
    local skipped=0
    
    echo -e "${YELLOW}Processing Shell script files...${NC}"
    
    find . -type f -name "*.sh" -not -path "*/\.*" | while read file; do
        if has_copyright_header "$file"; then
            skipped=$((skipped + 1))
        else
            # Check if file already has a shebang
            if grep -q "^#!/" "$file"; then
                # Extract the shebang
                shebang=$(head -n 1 "$file")
                # Remove the shebang from the file
                sed -i '1d' "$file"
                # Create a temporary header with the shebang
                temp_header="$shebang\n$(echo "$SHELL_COPYRIGHT_HEADER" | tail -n +2)"
                # Add the modified header
                add_header "$file" "$(echo -e "$temp_header")"
            else
                add_header "$file" "$SHELL_COPYRIGHT_HEADER"
            fi
            count=$((count + 1))
        fi
    done
    
    echo -e "${GREEN}Added headers to $count Shell script files. Skipped $skipped files that already had headers.${NC}"
}

# Process Makefiles
process_makefiles() {
    local count=0
    local skipped=0
    
    echo -e "${YELLOW}Processing Makefiles...${NC}"
    
    find . -type f \( -name "Makefile" -o -name "*.mk" \) -not -path "*/\.*" | while read file; do
        if has_copyright_header "$file"; then
            skipped=$((skipped + 1))
        else
            add_header "$file" "$MAKEFILE_COPYRIGHT_HEADER"
            count=$((count + 1))
        fi
    done
    
    echo -e "${GREEN}Added headers to $count Makefiles. Skipped $skipped files that already had headers.${NC}"
}

# Main execution
echo -e "${YELLOW}Starting copyright header addition process...${NC}"
echo -e "${YELLOW}Working directory: $(pwd)${NC}"

# Make sure we're in the project root
if [ ! -f "LICENSE" ] || [ ! -f "COPYRIGHT" ]; then
    echo -e "${RED}Error: LICENSE or COPYRIGHT file not found.${NC}"
    echo -e "${RED}Please run this script from the project root directory.${NC}"
    exit 1
fi

# Process different file types
process_c_files
process_asm_files
process_shell_files
process_makefiles

echo -e "${GREEN}Copyright header addition complete!${NC}"
echo -e "${YELLOW}Note: You may need to manually add headers to other file types.${NC}"

exit 0
