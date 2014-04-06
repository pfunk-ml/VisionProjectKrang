function mat = getMatrixFromEuler(theta, psi, phi, x, y, z)
    mat = makehgtform; 
    mat(1,1) = cos(theta)*cos(phi);
    mat(1,2) = sin(psi)*sin(theta)*cos(phi)-cos(psi)*sin(phi);
    mat(1,3) = cos(psi)*sin(theta)*cos(phi)+sin(psi)*sin(phi);
    mat(2,1) = cos(theta)*sin(phi);
    mat(2,2) = sin(psi)*sin(theta)*sin(phi)+cos(psi)*cos(phi);
    mat(2,3) = cos(psi)*sin(theta)*sin(phi)-sin(psi)*cos(phi);
    mat(3,1) = -sin(theta);
    mat(3,2) = sin(psi)*cos(theta);
    mat(3,3) = cos(psi)*cos(theta);
    mat(1,4) = x;
    mat(2,4) = y;
    mat(3,4) = z;
end
