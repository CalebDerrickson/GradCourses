function Plotting(VN_seq, NMHM_seq)
scr_siz = get(0,'ScreenSize') ;
ribbon_height = [0 40 0 0];
VN_numsteps = size(VN_seq, 1);
NMHM_numsteps = size(NMHM_seq, 1);
colorMap1 = parula(VN_numsteps);
colorMap2 = turbo(NMHM_numsteps);

% Separate VN_seq 
f = figure(1);
f.Position = [0, 0, scr_siz(3)/2, scr_siz(4)/2.2-25] + ribbon_height;
subplot(2, 1, 1);
plot(VN_seq(:, 1));
xlabel('Iteration'), ylabel('X - Position'), title('X - Position of Vanilla Newton Sequence')
grid on

subplot(2, 1, 2);
plot(VN_seq(:, 2));
xlabel('Iteration'), ylabel('Y - Position'), title('Y - Position of Vanilla Newton Sequence')
grid on

f2 = figure(2);
f2.Position = [0, scr_siz(4)/2 - 15, scr_siz(3)/2, scr_siz(4)/2.2-25] + ribbon_height;
subplot(2, 1, 1);
plot(NMHM_seq(:, 1));
xlabel('Iteration'), ylabel('X - Position'), title('X - Position of Modified Newton Sequence')
grid on

subplot(2, 1, 2);
plot(NMHM_seq(:, 2));
xlabel('Iteration'), ylabel('Y - Position'), title('Y - Position of Modified Newton Sequence')
grid on


f3 = figure(3);
f3.Position = [scr_siz(3)/2, 0, scr_siz(3)/2, scr_siz(4) - 89] + ribbon_height;

ax1 = axes;
hold on;
plot(VN_seq(:, 1), VN_seq(:, 2), 'Color', 'k', 'LineWidth', 1.25);
scatter(VN_seq(:, 1), VN_seq(:, 2), 50, colorMap1, 'filled');
axis equal

ax2 = axes;
hold on;
plot(NMHM_seq(:, 1), NMHM_seq(:, 2), 'Color', 'k', 'LineWidth', 1.25);
scatter(NMHM_seq(:, 1), NMHM_seq(:, 2), 50, colorMap2, 'filled');
plot(NMHM_seq(1, 1), NMHM_seq(1, 2), 'ko', 'MarkerSize', 15, 'LineWidth', 2);
axis equal

ax2.Visible = 'off';
ax2.XTick = [];
ax2.YTick = [];

colormap(ax1, "parula");
colormap(ax2, "turbo");
linkaxes([ax1, ax2])

cb1 = colorbar(ax1,'Position',[0.82 0.1 0.025 0.815]);
cb2 = colorbar(ax2,'Position',[0.92 0.1 0.025 0.815]);

clim(ax1, [0, round(VN_numsteps, 1, "significant")]);
clim(ax2, [0, round(NMHM_numsteps, 1, "significant")]);

cb1.Label.String ="Vanilla Newton Sequence Step";
cb2.Label.String ="Modified Newton Sequence Step";

grid on;


end