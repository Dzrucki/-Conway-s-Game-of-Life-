import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class GameOfLife extends JFrame {
    private final int gridSize = 50;
    private final int cellSize = 10;
    private boolean[][] cells = new boolean[gridSize][gridSize];
    private boolean running = false;

    public GameOfLife() {
        setTitle("Game of Life");
        setSize(gridSize * cellSize, gridSize * cellSize);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setResizable(false);

        JPanel gridPanel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                for (int y = 0; y < gridSize; y++) {
                    for (int x = 0; x < gridSize; x++) {
                        if (cells[y][x]) {
                            g.setColor(Color.WHITE);
                        } else {
                            g.setColor(Color.BLACK);
                        }
                        g.fillRect(x * cellSize, y * cellSize, cellSize, cellSize);
                        g.setColor(Color.GRAY);
                        g.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
                    }
                }
            }
        };

        gridPanel.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                int x = e.getX() / cellSize;
                int y = e.getY() / cellSize;
                cells[y][x] = !cells[y][x];
                gridPanel.repaint();
            }
        });

        Timer timer = new Timer(100, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (running) {
                    updateCells();
                    gridPanel.repaint();
                }
            }
        });
        timer.start();

        gridPanel.setPreferredSize(new Dimension(gridSize * cellSize, gridSize * cellSize));
        add(gridPanel, BorderLayout.CENTER);

        JPanel controlPanel = new JPanel();
        JButton startButton = new JButton("Start");
        startButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                running = !running;
                startButton.setText(running ? "Pause" : "Start");
            }
        });
        controlPanel.add(startButton);

        JButton resetButton = new JButton("Reset");
        resetButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                running = false;
                startButton.setText("Start");
                cells = new boolean[gridSize][gridSize];
                gridPanel.repaint();
            }
        });
        controlPanel.add(resetButton);

        add(controlPanel, BorderLayout.SOUTH);
    }

    private void updateCells() {
        boolean[][] newCells = new boolean[gridSize][gridSize];
        for (int y = 0; y < gridSize; y++) {
            for (int x = 0; x < gridSize; x++) {
                int neighbors = countNeighbors(y, x);
                if (cells[y][x]) {
                    newCells[y][x] = neighbors == 2 || neighbors == 3;
                } else {
                    newCells[y][x] = neighbors == 3;
                }
            }
        }
        cells = newCells;
    }

    private int countNeighbors(int y, int x) {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;
                int ny = y + i;
                int nx = x + j;
                if (ny >= 0 && ny < gridSize && nx >= 0 && nx < gridSize && cells[ny][nx]) {
                    count++;
                }
            }
        }
        return count;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                GameOfLife game = new GameOfLife();
                game.setVisible(true);
            }
        });
    }
}
