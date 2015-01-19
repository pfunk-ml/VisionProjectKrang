% Get average matrix
function avgmat = getAvgMatrix(filepath)
    
    % Open file for reading
    infh = fopen(filepath);
    line = fgets(infh);
    mats = {};
    mat = [];
    while(ischar(line))
        
        % Start new matrix
        if (strcmp(line(1:26), 'TRANSFORMATION OF OBJECT 0')) 
            if (~isempty(mat))
                mats = [mats {mat}];
                mat = [];
            end
            line = fgets(infh);
        end
        
        % Do conversion
        newdataline = '';
        [tok, rest] = strtok(line, [' ' 9 10 13]);
        row = [];
        while (~isempty(tok)) 
            num = str2double(tok);
            row = [row num];
            [tok, rest] = strtok(rest, [' ' 9 10 13]);
        end
        
        mat = [mat; row];
        
        % Get next line
        line = fgets(infh);
    end
    
    fclose(infh);
    
    % Now do averaging
    totx = 0;
    toty = 0;
    totz = 0;
    tottheta = 0;
    totpsi = 0;
    totphi = 0;
    numMats = length(mats);
    for i = 1:numMats
        mat = mats{i};
        [theta, psi, phi, x, y, z] = getEulerAngles(mat);
        
        totx = totx + x;
        toty = toty + y;
        totz = totz + z;
        tottheta = tottheta + theta;
        totpsi = totpsi + psi;
        totphi = totphi + phi;
    end
    
    avgx = totx / numMats;
    avgy = toty / numMats;
    avgz = totz / numMats;
    avgtheta = tottheta / numMats;
    avgpsi = totpsi / numMats;
    avgphi = totphi / numMats;
    
    avgmat = getMatrixFromEuler(avgtheta, avgpsi, avgphi, avgx, avgy, avgz);

end
