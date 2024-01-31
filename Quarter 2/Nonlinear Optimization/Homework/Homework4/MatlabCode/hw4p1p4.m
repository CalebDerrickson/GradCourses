function hw4p1p4(niter, eps)


global lin_systems_counter
lin_systems_counter = 0;

if isempty( lin_systems_counter )
    lin_systems_counter=0; %Initializing counter
end

[~,name,~]=fileparts(pwd);
if strcmp(name, 'MatlabCode') ~= 0
    cd ("..\")
end
    
Dogleg_time = zeros(1, niter);
Dogleg_lin_systems = zeros(1, niter);

NMHM_time = zeros(1, niter);
NMHM_lin_systems = zeros(1, niter);
x0 = 2.*ones(1, niter);


cd('MatlabCode\Dogleg')
for i = 2:niter
    [time, ~] = TrustRegion(@rosenbrockfgH, x0(1:i), eps);
    Dogleg_time(i) = time;
    Dogleg_lin_systems(i) = lin_systems_counter;
    lin_systems_counter = 0;
    fprintf("Dogleg %d\n", i / niter * 100);
end

lin_systems_counter = 0;
cd('..\NMHM')
for i = 2:niter
    [time, ~] = NMHM(@rosenbrockfgH, x0(1:i), eps);
    NMHM_time(i) = time;
    NMHM_lin_systems(i) = lin_systems_counter;
    lin_systems_counter = 0;
    fprintf("NMHM %d\n", i / niter * 100);
end

figure(1); hold on;
plot(Dogleg_time, DisplayName= 'Dogleg', LineWidth=2);
plot(NMHM_time, DisplayName='NMHM', LineWidth=2);
title("Timings of Dogleg and NMHM");
xlabel("Size $n$", Interpreter="latex");
ylabel("Time($s$)", Interpreter="latex");
grid on;
legend
hold off;

figure(2); hold on; 
plot(Dogleg_lin_systems, DisplayName='Dogleg', Linewidth = 2);
plot(NMHM_lin_systems, DisplayName='NMHM', LineWidth= 2);
title("Linear Systems Calculated");
xlabel("Size $n$", Interpreter="latex");
ylabel("Number", Interpreter='latex');
grid on;
ylim([0 510])
legend;
hold off;
end