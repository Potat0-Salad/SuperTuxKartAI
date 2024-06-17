import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
import pandas as pd
import json
import os
import numpy as np

# Define the neural network architecture
class ScoringModel(nn.Module):
    def __init__(self, num_inputs=29):
        super(ScoringModel, self).__init__()
        self.layer1 = nn.Linear(num_inputs, 512)
        self.layer2 = nn.Linear(512, 256)
        self.layer3 = nn.Linear(256, 128)
        self.layer4 = nn.Linear(128, 64)
        self.layer5 = nn.Linear(64, 32)
        self.output_layer = nn.Linear(32, 1)
        self.leaky_relu = nn.LeakyReLU()
        self.dropout = nn.Dropout(0.5)

    def forward(self, x):
        x = self.leaky_relu(self.layer1(x))
        x = self.dropout(x)
        x = self.leaky_relu(self.layer2(x))
        x = self.dropout(x)
        x = self.leaky_relu(self.layer3(x))
        x = self.dropout(x)
        x = self.leaky_relu(self.layer4(x))
        x = self.dropout(x)
        x = self.leaky_relu(self.layer5(x))
        x = self.dropout(x)
        x = torch.sigmoid(self.output_layer(x))
        return x

# Function to load experiences from JSON file
def load_experiences(path):
    with open(path, 'r') as f:
        experiences_json = json.load(f)
    
    experiences = []
    for exp in experiences_json:
        experiences.append((torch.tensor(exp['state']), exp['action'], exp['reward'], torch.tensor(exp['next_state']), exp['done']))
    return experiences

# Function to prepare data from experiences
def prepare_data(experiences):
    states = [exp[0] for exp in experiences]
    actions = [exp[1] for exp in experiences]
    rewards = [exp[2] for exp in experiences]
    next_states = [exp[3] for exp in experiences]
    dones = [exp[4] for exp in experiences]
    
    states = torch.stack(states)
    actions = torch.tensor(actions)
    rewards = torch.tensor(rewards).float().unsqueeze(1)  # Ensure rewards are of shape [batch_size, 1]
    next_states = torch.stack(next_states)
    dones = torch.tensor(dones).float().unsqueeze(1)  # Ensure dones are of shape [batch_size, 1]

    # Print the raw rewards
    print("Raw rewards:", rewards)
    
    # Handle NaN values
    rewards = torch.nan_to_num(rewards, nan=0.0, posinf=1.0, neginf=0.0)

    # Normalize rewards to be between 0 and 1
    if rewards.max() != rewards.min():  # Avoid division by zero
        rewards = (rewards - rewards.min()) / (rewards.max() - rewards.min())
    rewards = torch.clamp(rewards, 0, 1)  # Clamp rewards to be in the range [0, 1]

    print("Rewards range after normalization and clamping:", rewards.min().item(), rewards.max().item())
    
    return states, actions, rewards, next_states, dones

# Define the loss function and optimizer
criterion = nn.BCELoss()
optimizer = None

# Training function for fine-tuning the model
def train_model(train_loader, model, criterion, optimizer, num_epochs=500):
    model.train()
    for epoch in range(num_epochs):
        for inputs, labels in train_loader:
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
        if (epoch + 1) % 10 == 0:  # Print every 10 epochs
            print(f'Epoch {epoch+1}/{num_epochs}, Loss: {loss.item():.4f}')

# Main function to load experiences and train the model
def main():
    experiences_path = "/Users/marcel/Desktop/project/model/experiences.json"
    model_path = "/Users/marcel/Desktop/project/model/soccer_ai_model.pt"

    print(f"Loading experiences from: {experiences_path}")
    # Load experiences
    experiences = load_experiences(experiences_path)
    print(f"Loaded experiences: {len(experiences)}")

    # Prepare data
    states, actions, rewards, next_states, dones = prepare_data(experiences)
    print("Prepared data")

    # Convert to PyTorch DataLoader
    dataset = TensorDataset(states, rewards)  # Assuming rewards are used as labels for simplicity
    train_loader = DataLoader(dataset, batch_size=64, shuffle=True)

    # Initialize the model
    global model, optimizer
    model = ScoringModel(num_inputs=29)
    print("Model initialized")

    # Load the model state if it exists
    if os.path.isfile(model_path):
        loaded_model = torch.jit.load(model_path)
        model.load_state_dict(loaded_model.state_dict())
        print(f"Model loaded from: {model_path}")
        print("Model state_dict loaded")

    optimizer = optim.Adam(model.parameters(), lr=0.001)

    print("Starting training")
    # Train the model
    train_model(train_loader, model, criterion, optimizer)
    print("Training complete")

    # Save the updated model
    torch.jit.save(torch.jit.script(model), model_path)
    print(f"Model saved to: {model_path}")

if __name__ == "__main__":
    main()
