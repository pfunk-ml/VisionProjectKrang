function [theta, psi, phi, x, y, z] = getEulerAngles(mat)
    x = mat(1, 4);
    y = mat(2, 4);
    z = mat(3, 4);
    theta1 = -asin(mat(3,1));
    theta2 = pi - theta1;
    theta = max(theta1, theta2);
    psi = atan2(mat(3,2)/cos(theta), mat(3,3)/cos(theta));
    phi = atan2(mat(2,1)/cos(theta), mat(1,1)/cos(theta));
end
